import filecmp
import anybadge
import re
import os
import subprocess
import sys
import json
import multiprocessing
import io
from contextlib import redirect_stderr, redirect_stdout

LLVM_HISTORY_FILE = ".llvm_history.json"
MIPS_HISTORY_FILE = ".mips_history.json"
TIMEOUT_SECONDS = 2
EXPECTED_TO_FAIL_FILES = {
    "added_tests/video_test7.c": "Negative test: Semantic errors",
    "added_tests/video_test9.c": "Negative test: Missing return path",
}


def load_history(filename):
    try:
        with open(filename, "r") as f:
            return set(json.load(f))
    except (FileNotFoundError, json.JSONDecodeError):
        return set()


def save_history(filename, successful_paths):
    with open(filename, "w") as f:
        json.dump(list(successful_paths), f, indent=4)


def _run_compiler_process(config, queue, use_mips=False):
    output_capture = io.StringIO()
    try:
        with redirect_stdout(output_capture), redirect_stderr(output_capture):
            from src.driver import CompilerPipeline

            pipeline = CompilerPipeline(config)
            pipeline.run()
        # SUCCESS is when no fatal errors occurred
        success = not pipeline._has_fatal_errors()
        queue.put((success, output_capture.getvalue()))
    except Exception as e:
        queue.put((False, f"Exception: {str(e)}\n{output_capture.getvalue()}"))


def run_tests(test_dir="src/tests", results_dir="test_results"):
    from src.driver import PipelineConfig

    test_dir = os.path.abspath(test_dir)
    results_dir = os.path.abspath(results_dir)
    os.makedirs(results_dir, exist_ok=True)

    llvm_previous_successes = load_history(LLVM_HISTORY_FILE)
    mips_previous_successes = load_history(MIPS_HISTORY_FILE)

    llvm_current_successes = set()
    mips_current_successes = set()

    llvm_regression_info = {}
    mips_regression_info = {}

    stats = {
        "llvm_both_exist_but_different": 0,
        "llvm_bothfail": 0,
        "llvm_caused_exception": 0,
        "llvm_expected_fails_passed": 0,
        "llvm_gccfail_ourpass": 0,
        "llvm_gccpass_ourfail": 0,
        "llvm_pass_same_answer": 0,
        "llvm_timed_out_count": 0,
        "llvm_total_count": 0,
        "llvm_unexpected_successes": 0,
        "mips_both_exist_but_different": 0,
        "mips_bothfail": 0,
        "mips_caused_exception": 0,
        "mips_expected_fails_passed": 0,
        "mips_gccfail_ourpass": 0,
        "mips_gccpass_ourfail": 0,
        "mips_pass_same_answer": 0,
        "mips_timed_out_count": 0,
        "mips_total_count": 0,
        "mips_unexpected_successes": 0,
        "skipped": 0,
    }

    for root, dirs, files in os.walk(test_dir):
        for file in files:
            if not file.endswith(".c"):
                continue

            file_path = os.path.join(root, file)
            rel_file_path = os.path.relpath(file_path, test_dir)
            normalized_rel_path = rel_file_path.replace("\\", "/")

            SKIPPED_PATHS = [
                "FunctionPtrTests",
                "dynamic-test.c",
                "test_set_3/ASTTests/ConversionTests/test16.c",
                "test_set_3/LLVMTests/FileIOTests/test7.c",
                "test_set_3/LLVMTests/FunctionTests/test51.c",
                "test_set_3/LLVMTests/PrintTests/test16.c",
                "test_set_3/LLVMTests/ScanTests/test8.c",
                "test_set_3/MipsTests/ArrayTests/test59.c",
                "test_set_3/MipsTests/CalculationTests/test1.c",
                "test_set_3/MipsTests/CalculationTests/test3.c",
                "test_set_3/MipsTests/FileIOTests/test7.c",
                "test_set_3/MipsTests/FunctionTests/test12.c",
                "test_set_3/MipsTests/FunctionTests/test20.c",
                "test_set_3/MipsTests/FunctionTests/test51.c",
                "test_set_3/MipsTests/PrintTests/test10.c",
                "test_set_3/MipsTests/PrintTests/test45.c",
                "test_set_3/LLVMTests/PrintTests/test15.c",
                "test_set_1/test_file_36.c",
                "test_set_3/LLVMTests/FileIOTests/test8.c",
            ]

            if any(excluded in normalized_rel_path for excluded in SKIPPED_PATHS):
                stats["skipped"] += 1
                print(f"[ SKIPPED ] {rel_file_path}")
                continue

            target_result_dir = os.path.join(
                results_dir, os.path.dirname(rel_file_path)
            )
            os.makedirs(target_result_dir, exist_ok=True)
            base_name = os.path.splitext(file)[0]

            with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
                content = f.read()

            is_scanf_test = "scanf" in content
            inputs_to_test = (
                [""]
                if not is_scanf_test
                else ["42 99 100\n", "0 1 2\n", "-5 -10 -15\n"]
            )
            expected_to_fail = normalized_rel_path in EXPECTED_TO_FAIL_FILES

            stats["llvm_total_count"] += 1
            stats["mips_total_count"] += 1

            exec_path = os.path.join(root, f"{base_name}")
            ll_path = os.path.join(root, f"{base_name}.ll")
            mips_path = os.path.join(root, f"{base_name}.s")
            llvm_output_dest = os.path.join(
                target_result_dir, f"{base_name}_llvm_output.txt"
            )
            mips_output_dest = os.path.join(
                target_result_dir, f"{base_name}_mips_output.txt"
            )
            reference_output_path = os.path.join(
                target_result_dir, f"{base_name}_reference_output.txt"
            )

            # Clean up potential leftovers from a prior execution immediately
            local_artifacts = [
                exec_path,
                ll_path,
                mips_path,
                llvm_output_dest,
                mips_output_dest,
                reference_output_path,
            ]
            for art in local_artifacts:
                if os.path.exists(art):
                    os.remove(art)

            # --- LLVM/lli test path ---
            config_llvm = PipelineConfig(
                file_path=file_path,
                llvmir_output=ll_path,
                print_errors=False,
                print_info=False,
            )
            llvm_queue = multiprocessing.Queue()
            p_llvm = multiprocessing.Process(
                target=_run_compiler_process, args=(config_llvm, llvm_queue)
            )
            p_llvm.start()
            p_llvm.join(TIMEOUT_SECONDS)

            our_compiler_llvm_success = False
            our_llvm_timeout = False
            llvm_compiler_output = ""

            if p_llvm.is_alive():
                p_llvm.terminate()
                p_llvm.join()
                our_llvm_timeout = True
            elif not llvm_queue.empty():
                our_compiler_llvm_success, llvm_compiler_output = llvm_queue.get()

            llvm_run_executed = False
            if our_compiler_llvm_success and os.path.exists(ll_path):
                try:
                    for i, test_input in enumerate(inputs_to_test):
                        run_result = subprocess.run(
                            ["lli", ll_path],
                            input=test_input,
                            capture_output=True,
                            text=True,
                            encoding="utf-8",
                            errors="replace",
                            timeout=TIMEOUT_SECONDS,
                        )
                        mode = "w" if i == 0 else "a"
                        with open(llvm_output_dest, mode, encoding="utf-8") as f:
                            if is_scanf_test:
                                f.write(
                                    f"--- Test {i + 1} [Input: {test_input.strip()}] ---\n"
                                )
                            f.write(run_result.stdout)
                    llvm_run_executed = True
                except subprocess.TimeoutExpired:
                    our_llvm_timeout = True
                except Exception:
                    stats["llvm_caused_exception"] += 1

            # --- MIPS codegen test path ---
            config_mips = PipelineConfig(
                file_path=file_path,
                mips_output=mips_path,
                print_errors=False,
                print_info=False,
            )
            mips_queue = multiprocessing.Queue()
            p_mips = multiprocessing.Process(
                target=_run_compiler_process, args=(config_mips, mips_queue, True)
            )
            p_mips.start()
            p_mips.join(TIMEOUT_SECONDS)

            mips_success = False
            mips_timeout = False
            mips_compiler_output = ""

            if p_mips.is_alive():
                p_mips.terminate()
                p_mips.join()
                mips_timeout = True
            elif not mips_queue.empty():
                mips_success, mips_compiler_output = mips_queue.get()

            mips_run_executed = False
            if mips_success and os.path.exists(mips_path):
                try:
                    for i, test_input in enumerate(inputs_to_test):
                        run_result = subprocess.run(
                            ["spim", "-file", mips_path],
                            input=test_input,
                            capture_output=True,
                            text=True,
                            encoding="utf-8",
                            errors="replace",
                            timeout=TIMEOUT_SECONDS,
                        )
                        mode = "w" if i == 0 else "a"
                        with open(mips_output_dest, mode, encoding="utf-8") as f:
                            if is_scanf_test:
                                f.write(
                                    f"--- Test {i + 1} [Input: {test_input.strip()}] ---\n"
                                )
                            output_body = re.sub(
                                r"(?s)^.*?Loaded:.*?\n", "", run_result.stdout
                            )
                            f.write(output_body)
                    mips_run_executed = True
                except subprocess.TimeoutExpired:
                    mips_timeout = True
                except Exception:
                    stats["mips_caused_exception"] += 1

            # --- GCC Reference ---
            gcc_status, _ = _try_gcc_compilation(file_path, exec_path)
            ref_timeout = gcc_status == "timeout"

            ref_run_executed = False
            if (
                not ref_timeout
                and gcc_status == "success"
                and os.path.exists(exec_path)
            ):
                try:
                    for i, test_input in enumerate(inputs_to_test):
                        run_result = subprocess.run(
                            [exec_path],
                            input=test_input,
                            capture_output=True,
                            text=True,
                            encoding="utf-8",
                            errors="replace",
                            timeout=TIMEOUT_SECONDS,
                        )
                        mode = "w" if i == 0 else "a"
                        with open(reference_output_path, mode, encoding="utf-8") as f:
                            if is_scanf_test:
                                f.write(
                                    f"--- Test {i + 1} [Input: {test_input.strip()}] ---\n"
                                )
                            f.write(run_result.stdout)
                    ref_run_executed = True
                except subprocess.TimeoutExpired:
                    ref_timeout = True

            # ==========================================
            # --- LLVM Verification & Printing Logic ---
            # ==========================================
            if our_llvm_timeout and ref_timeout:
                stats["llvm_pass_same_answer"] += 1
                llvm_current_successes.add(rel_file_path)
            elif our_llvm_timeout or ref_timeout:
                stats["llvm_timed_out_count"] += 1
                llvm_regression_info[rel_file_path] = "LLVM or Reference Timeout"
                print(f"[ TIME (LLVM) ] {rel_file_path}")
            elif expected_to_fail:
                if not our_compiler_llvm_success:
                    stats["llvm_expected_fails_passed"] += 1
                    llvm_current_successes.add(rel_file_path)
                else:
                    stats["llvm_unexpected_successes"] += 1
                    llvm_regression_info[rel_file_path] = (
                        "LLVM Failed to fail (unexpected LLVM success)"
                    )
                    print(
                        f"[ LLVM FAIL ] {rel_file_path} — expected failure, but succeeded"
                    )
            elif gcc_status == "success":
                if not llvm_run_executed or not our_compiler_llvm_success:
                    stats["llvm_gccpass_ourfail"] += 1
                    llvm_regression_info[rel_file_path] = (
                        "GCC passed, our LLVM compiler failed"
                    )
                    print(f"[ LLVM FAIL (COMPILE ERROR) ] {rel_file_path}")
                    if llvm_compiler_output.strip():
                        print(
                            f"  --- COMPILER OUTPUT ---\n  {llvm_compiler_output.strip().replace(chr(10), chr(10) + '  ')}"
                        )
                else:
                    if filecmp.cmp(
                        llvm_output_dest, reference_output_path, shallow=False
                    ):
                        stats["llvm_pass_same_answer"] += 1
                        llvm_current_successes.add(rel_file_path)
                    else:
                        stats["llvm_both_exist_but_different"] += 1
                        llvm_regression_info[rel_file_path] = "LLVM output mismatch"
                        print(f"[ LLVM DIFF ] {rel_file_path}")
                        try:
                            with (
                                open(reference_output_path, "r") as rf,
                                open(llvm_output_dest, "r") as lf,
                            ):
                                print(
                                    f"  --- EXPECTED (GCC) ---\n  {rf.read().strip().replace(chr(10), chr(10) + '  ')}"
                                )
                                print(
                                    f"  --- ACTUAL (LLVM) ---\n  {lf.read().strip().replace(chr(10), chr(10) + '  ')}"
                                )
                        except Exception as e:
                            print(f"  (Could not read outputs: {e})")
            else:
                if not llvm_run_executed:
                    stats["llvm_bothfail"] += 1
                    llvm_current_successes.add(rel_file_path)
                else:
                    stats["llvm_gccfail_ourpass"] += 1
                    llvm_regression_info[rel_file_path] = "GCC failed, LLVM passed"
                    print(f"[ LLVM FAIL (GCC STRICTER) ] {rel_file_path}")

            # ==========================================
            # --- MIPS Verification & Printing Logic ---
            # ==========================================
            if mips_timeout and ref_timeout:
                stats["mips_pass_same_answer"] += 1
                mips_current_successes.add(rel_file_path)
            elif mips_timeout or ref_timeout:
                stats["mips_timed_out_count"] += 1
                mips_regression_info[rel_file_path] = "MIPS or Reference Timeout"
                print(f"[ TIME (MIPS) ] {rel_file_path}")
            elif expected_to_fail:
                if not mips_success:
                    stats["mips_expected_fails_passed"] += 1
                    mips_current_successes.add(rel_file_path)
                else:
                    stats["mips_unexpected_successes"] += 1
                    mips_regression_info[rel_file_path] = (
                        "MIPS Failed to fail (unexpected MIPS success)"
                    )
                    print(
                        f"[ MIPS FAIL ] {rel_file_path} — expected failure, but succeeded"
                    )
            elif gcc_status == "success":
                if not mips_run_executed or not mips_success:
                    stats["mips_gccpass_ourfail"] += 1
                    mips_regression_info[rel_file_path] = (
                        "GCC passed, our MIPS compiler failed"
                    )
                    print(f"[ MIPS FAIL (COMPILE ERROR) ] {rel_file_path}")
                    if mips_compiler_output.strip():
                        print(
                            f"  --- COMPILER OUTPUT ---\n  {mips_compiler_output.strip().replace(chr(10), chr(10) + '  ')}"
                        )
                else:
                    ref_for_mips = (
                        reference_output_path if ref_run_executed else llvm_output_dest
                    )
                    if os.path.exists(ref_for_mips) and filecmp.cmp(
                        mips_output_dest, ref_for_mips, shallow=False
                    ):
                        stats["mips_pass_same_answer"] += 1
                        mips_current_successes.add(rel_file_path)
                    else:
                        stats["mips_both_exist_but_different"] += 1
                        mips_regression_info[rel_file_path] = "MIPS output mismatch"
                        print(f"[ MIPS DIFF ] {rel_file_path}")
                        try:
                            if os.path.exists(ref_for_mips):
                                with (
                                    open(ref_for_mips, "r") as rf,
                                    open(mips_output_dest, "r") as mf,
                                ):
                                    print(
                                        f"  --- EXPECTED (GCC/LLVM) ---\n  {rf.read().strip().replace(chr(10), chr(10) + '  ')}"
                                    )
                                    print(
                                        f"  --- ACTUAL (MIPS) ---\n  {mf.read().strip().replace(chr(10), chr(10) + '  ')}"
                                    )
                        except Exception as e:
                            print(f"  (Could not read outputs: {e})")
            else:
                if not mips_run_executed:
                    stats["mips_bothfail"] += 1
                    mips_current_successes.add(rel_file_path)
                else:
                    stats["mips_gccfail_ourpass"] += 1
                    mips_regression_info[rel_file_path] = "GCC failed, MIPS passed"
                    print(f"[ MIPS FAIL (GCC STRICTER) ] {rel_file_path}")

            # Clean up local generated artifacts right away so they don't impact subsequent loop runs
            for art in local_artifacts:
                if os.path.exists(art):
                    os.remove(art)

    # History & Aggregates Compilation
    save_history(LLVM_HISTORY_FILE, llvm_current_successes)
    save_history(MIPS_HISTORY_FILE, mips_current_successes)

    llvm_regressions = llvm_previous_successes - llvm_current_successes
    llvm_newly_working = llvm_current_successes - llvm_previous_successes
    passed_total_llvm = (
        stats["llvm_pass_same_answer"]
        + stats["llvm_bothfail"]
        + stats["llvm_expected_fails_passed"]
    )

    mips_regressions = mips_previous_successes - mips_current_successes
    mips_newly_working = mips_current_successes - mips_previous_successes
    passed_total_mips = (
        stats["mips_pass_same_answer"]
        + stats["mips_bothfail"]
        + stats["mips_expected_fails_passed"]
    )

    # Generate Badges
    generate_detailed_badges(
        llvm_total=stats["llvm_total_count"],
        llvm_passed=passed_total_llvm,
        llvm_regressions=len(llvm_regressions),
        llvm_timeouts=stats["llvm_timed_out_count"],
        llvm_gcc_stricter=stats["llvm_gccfail_ourpass"],
        llvm_exceptions=stats["llvm_caused_exception"],
        llvm_missing_features=stats["llvm_gccpass_ourfail"],
        llvm_diff_outputs=stats["llvm_both_exist_but_different"],
        llvm_unexpected_successes=stats["llvm_unexpected_successes"],
        llvm_newly_working=len(llvm_newly_working),
        skipped=stats["skipped"],
        mips_total=stats["mips_total_count"],
        mips_passed=passed_total_mips,
        mips_regressions=len(mips_regressions),
        mips_timeouts=stats["mips_timed_out_count"],
        mips_gcc_stricter=stats["mips_gccfail_ourpass"],
        mips_exceptions=stats["mips_caused_exception"],
        mips_missing_features=stats["mips_gccpass_ourfail"],
        mips_diff_outputs=stats["mips_both_exist_but_different"],
        mips_unexpected_successes=stats["mips_unexpected_successes"],
        mips_newly_working=len(mips_newly_working),
    )

    # --- FINAL SYNOPSIS ---
    print("\n" + "." * 40)
    print(f"--- LLVM {passed_total_llvm} / {stats['llvm_total_count']} passed ---")
    print(f"LLVM agreement on failure: {stats['llvm_bothfail']}")
    print(f"LLVM agreement on success: {stats['llvm_pass_same_answer']}")
    print(
        f"LLVM expected compile failures passed: {stats['llvm_expected_fails_passed']}"
    )
    print(f"Skipped {stats['skipped']} files")
    print("." * 25)
    print(f"LLVM Total Timeouts: {stats['llvm_timed_out_count']}")
    print(f"LLVM Newly Working: {len(llvm_newly_working)}")

    print("\n" + "." * 40)
    print(f"--- MIPS {passed_total_mips} / {stats['mips_total_count']} passed ---")
    print(f"MIPS agreement on failure: {stats['mips_bothfail']}")
    print(f"MIPS agreement on success: {stats['mips_pass_same_answer']}")
    print(
        f"MIPS expected compile failures passed: {stats['mips_expected_fails_passed']}"
    )
    print("." * 25)
    print(f"MIPS Total Timeouts: {stats['mips_timed_out_count']}")
    print(f"MIPS Newly Working: {len(mips_newly_working)}")
    print("." * 40)

    if llvm_newly_working:
        print("\n*** NEWLY WORKING LLVM TESTS ***")
        for nw in sorted(llvm_newly_working):
            print(f"  + {nw}")

    if mips_newly_working:
        print("\n*** NEWLY WORKING MIPS TESTS ***")
        for nw in sorted(mips_newly_working):
            print(f"  + {nw}")

    if llvm_regressions:
        print("\n!!! LLVM REGRESSIONS DETECTED !!!")
        for r in sorted(llvm_regressions):
            reason = llvm_regression_info.get(
                r, "Previously passed, now failed execution/output test"
            )
            print(f"  - {r} (Reason: {reason})")

    if mips_regressions:
        print("\n!!! MIPS REGRESSIONS DETECTED !!!")
        for r in sorted(mips_regressions):
            reason = mips_regression_info.get(
                r, "Previously passed, now failed execution/output test"
            )
            print(f"  - {r} (Reason: {reason})")

    sys.exit(0)


def generate_detailed_badges(*args, **kwargs):
    os.makedirs("badges", exist_ok=True)

    def make_badge(label, value, color, filename):
        badge = anybadge.Badge(label, str(value), default_color=color)
        badge.write_badge(f"badges/{filename}.svg", overwrite=True)

    make_badge(
        "LLVM Tests Passed",
        f"{kwargs['llvm_passed']}/{kwargs['llvm_total']}",
        "green" if kwargs["llvm_passed"] == kwargs["llvm_total"] else "orange",
        "llvm-tests-passed",
    )
    make_badge(
        "LLVM Regressions",
        kwargs["llvm_regressions"],
        "red" if kwargs["llvm_regressions"] > 0 else "green",
        "llvm-regressions",
    )
    make_badge(
        "LLVM Timeouts",
        kwargs["llvm_timeouts"],
        "red" if kwargs["llvm_timeouts"] > 0 else "green",
        "llvm-timeouts",
    )
    make_badge(
        "LLVM GCC Stricter", kwargs["llvm_gcc_stricter"], "yellow", "llvm-gcc-stricter"
    )
    make_badge(
        "LLVM Exceptions",
        kwargs["llvm_exceptions"],
        "red" if kwargs["llvm_exceptions"] > 0 else "green",
        "llvm-exceptions",
    )
    make_badge(
        "LLVM Missing Features",
        kwargs["llvm_missing_features"],
        "orange" if kwargs["llvm_missing_features"] > 0 else "green",
        "llvm-missing-features",
    )
    make_badge(
        "LLVM Different Outputs",
        kwargs["llvm_diff_outputs"],
        "orange" if kwargs["llvm_diff_outputs"] > 0 else "green",
        "llvm-diff-outputs",
    )
    make_badge(
        "LLVM Unexpected Success",
        kwargs["llvm_unexpected_successes"],
        "red" if kwargs["llvm_unexpected_successes"] > 0 else "green",
        "llvm-unexpected-success",
    )
    make_badge(
        "LLVM Newly Working",
        kwargs["llvm_newly_working"],
        "blue" if kwargs["llvm_newly_working"] > 0 else "lightgrey",
        "llvm-newly-working",
    )
    make_badge(
        "Skipped",
        kwargs["skipped"],
        "white" if kwargs["skipped"] > 0 else "lightgrey",
        "skipped",
    )

    make_badge(
        "MIPS Tests Passed",
        f"{kwargs['mips_passed']}/{kwargs['mips_total']}",
        "green" if kwargs["mips_passed"] == kwargs["mips_total"] else "orange",
        "mips-tests-passed",
    )
    make_badge(
        "MIPS Regressions",
        kwargs["mips_regressions"],
        "red" if kwargs["mips_regressions"] > 0 else "green",
        "mips-regressions",
    )
    make_badge(
        "MIPS Timeouts",
        kwargs["mips_timeouts"],
        "red" if kwargs["mips_timeouts"] > 0 else "green",
        "mips-timeouts",
    )
    make_badge(
        "MIPS GCC Stricter", kwargs["mips_gcc_stricter"], "yellow", "mips-gcc-stricter"
    )
    make_badge(
        "MIPS Exceptions",
        kwargs["mips_exceptions"],
        "red" if kwargs["mips_exceptions"] > 0 else "green",
        "mips-exceptions",
    )
    make_badge(
        "MIPS Missing Features",
        kwargs["mips_missing_features"],
        "orange" if kwargs["mips_missing_features"] > 0 else "green",
        "mips-missing-features",
    )
    make_badge(
        "MIPS Different Outputs",
        kwargs["mips_diff_outputs"],
        "orange" if kwargs["mips_diff_outputs"] > 0 else "green",
        "mips-diff-outputs",
    )
    make_badge(
        "MIPS Unexpected Success",
        kwargs["mips_unexpected_successes"],
        "red" if kwargs["mips_unexpected_successes"] > 0 else "green",
        "mips-unexpected-success",
    )
    make_badge(
        "MIPS Newly Working",
        kwargs["mips_newly_working"],
        "blue" if kwargs["mips_newly_working"] > 0 else "lightgrey",
        "mips-newly-working",
    )


def _try_gcc_compilation(file_path, exec_path):
    temp_ansi_file = file_path.replace(".c", ".ansi.c")
    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
        content = re.sub(r"//.*", "", f.read())
    with open(temp_ansi_file, "w", encoding="utf-8") as f:
        f.write(content)
    try:
        res = subprocess.run(
            [
                "gcc",
                "-ansi",
                "-pedantic",
                "-Wno-int-conversion",
                temp_ansi_file,
                "-o",
                exec_path,
            ],
            capture_output=True,
            text=True,
            timeout=TIMEOUT_SECONDS,
        )
        return ("success", "") if os.path.exists(exec_path) else ("fail", res.stderr)
    except subprocess.TimeoutExpired:
        return ("timeout", "")
    finally:
        if os.path.exists(temp_ansi_file):
            os.remove(temp_ansi_file)


if __name__ == "__main__":
    multiprocessing.freeze_support()
    run_tests()
