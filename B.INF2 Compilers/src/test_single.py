import os
import sys
import subprocess
import multiprocessing
import filecmp
import re

from src.driver import PipelineConfig
from src.test_llvm import (
    _try_gcc_compilation,
    _run_compiler_process,
    TIMEOUT_SECONDS,
)


def test_single_file(file_path):
    if not os.path.exists(file_path):
        print(f"Error: File {file_path} not found.")
        return

    file_path = os.path.abspath(file_path)
    base_name = os.path.splitext(os.path.basename(file_path))[0]
    results_dir = os.path.abspath("single_test_results")
    os.makedirs(results_dir, exist_ok=True)

    # Output artifact targets
    ll_path = os.path.join(results_dir, f"{base_name}.ll")
    mips_path = os.path.join(results_dir, f"{base_name}.s")
    exec_path = os.path.join(results_dir, f"{base_name}.out")
    llvm_output_txt = os.path.join(results_dir, f"{base_name}_llvm_output.txt")
    mips_output_txt = os.path.join(results_dir, f"{base_name}_mips_output.txt")
    ref_output_txt = os.path.join(results_dir, f"{base_name}_ref_output.txt")
    ast_path = os.path.join(results_dir, f"{base_name}.png")

    print(f"\n--- Testing: {os.path.basename(file_path)} ---")

    # Read and analyze file contents for dynamic inputs
    with open(file_path, "r", encoding="utf-8", errors="ignore") as f:
        content = f.read()

    is_scanf_test = "scanf" in content
    inputs_to_test = (
        [""] if not is_scanf_test else ["42 99 100\n", "0 1 2\n", "-5 -10 -15\n"]
    )

    # Defensive Cleanup of old run artifacts
    local_artifacts = [
        ll_path,
        mips_path,
        exec_path,
        llvm_output_txt,
        mips_output_txt,
        ref_output_txt,
        ast_path,
    ]
    for art in local_artifacts:
        if os.path.exists(art):
            os.remove(art)

    # ==========================================
    # --- LLVM Code Generation & Execution ---
    # ==========================================
    config_llvm = PipelineConfig(
        file_path=file_path,
        llvmir_output=ll_path,
        print_errors=True,
        print_info=True,
        ast_output=ast_path,
    )

    llvm_queue = multiprocessing.Queue()
    p_llvm = multiprocessing.Process(
        target=_run_compiler_process, args=(config_llvm, llvm_queue)
    )
    p_llvm.start()
    p_llvm.join(TIMEOUT_SECONDS)

    our_compiler_llvm_success = False
    llvm_compiler_log = ""

    if p_llvm.is_alive():
        p_llvm.terminate()
        p_llvm.join()
        print("[ TIME ] Our LLVM compiler timed out.")
    elif not llvm_queue.empty():
        our_compiler_llvm_success, llvm_compiler_log = llvm_queue.get()

    llvm_run_executed = False
    if our_compiler_llvm_success and os.path.exists(ll_path):
        try:
            for i, test_input in enumerate(inputs_to_test):
                res = subprocess.run(
                    ["lli", ll_path],
                    input=test_input,
                    capture_output=True,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=TIMEOUT_SECONDS,
                )
                mode = "w" if i == 0 else "a"
                with open(llvm_output_txt, mode, encoding="utf-8") as f:
                    if is_scanf_test:
                        f.write(f"--- Test {i + 1} [Input: {test_input.strip()}] ---\n")
                    f.write(res.stdout)
            print("[ INFO ] Our LLVM compiler generated output.")
            llvm_run_executed = True
        except subprocess.TimeoutExpired:
            print("[ TIME ] lli execution timed out.")
        except Exception as e:
            print(f"[ FAIL ] lli execution failed: {e}")
    else:
        print(
            "[ FAIL ] Our LLVM compiler failed to produce .ll file or returned failure."
        )
        if llvm_compiler_log:
            print(f"--- LLVM Compiler Log ---\n{llvm_compiler_log}")

    # ==========================================
    # --- MIPS Code Generation & Execution ---
    # ==========================================
    config_mips = PipelineConfig(
        file_path=file_path,
        mips_output=mips_path,
        print_errors=True,
        print_info=True,
    )

    mips_queue = multiprocessing.Queue()
    p_mips = multiprocessing.Process(
        target=_run_compiler_process, args=(config_mips, mips_queue, True)
    )
    p_mips.start()
    p_mips.join(TIMEOUT_SECONDS)

    mips_success = False
    mips_compiler_log = ""

    if p_mips.is_alive():
        p_mips.terminate()
        p_mips.join()
        print("[ TIME ] Our MIPS compiler timed out.")
    elif not mips_queue.empty():
        mips_success, mips_compiler_log = mips_queue.get()

    mips_run_executed = False
    if mips_success and os.path.exists(mips_path):
        try:
            for i, test_input in enumerate(inputs_to_test):
                res = subprocess.run(
                    ["spim", "-file", mips_path],
                    input=test_input,
                    capture_output=True,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=TIMEOUT_SECONDS,
                )
                mode = "w" if i == 0 else "a"
                with open(mips_output_txt, mode, encoding="utf-8") as f:
                    if is_scanf_test:
                        f.write(f"--- Test {i + 1} [Input: {test_input.strip()}] ---\n")
                    output_body = re.sub(r"(?s)^.*?Loaded:.*?\n", "", res.stdout)
                    f.write(output_body)
            print("[ INFO ] Our MIPS compiler generated output.")
            mips_run_executed = True
        except subprocess.TimeoutExpired:
            print("[ TIME ] spim execution timed out.")
        except Exception as e:
            print(f"[ FAIL ] spim execution failed: {e}")
    else:
        print(
            "[ FAIL ] Our MIPS compiler failed to produce .s file or returned failure."
        )
        if mips_compiler_log:
            print(f"--- MIPS Compiler Log ---\n{mips_compiler_log}")

    # ==========================================
    # --- GCC Reference Execution ---
    # ==========================================
    gcc_status, gcc_log = _try_gcc_compilation(file_path, exec_path)
    ref_run_executed = False

    if gcc_status == "success" and os.path.exists(exec_path):
        try:
            for i, test_input in enumerate(inputs_to_test):
                res = subprocess.run(
                    [exec_path],
                    input=test_input,
                    capture_output=True,
                    text=True,
                    encoding="utf-8",
                    errors="replace",
                    timeout=TIMEOUT_SECONDS,
                )
                mode = "w" if i == 0 else "a"
                with open(ref_output_txt, mode, encoding="utf-8") as f:
                    if is_scanf_test:
                        f.write(f"--- Test {i + 1} [Input: {test_input.strip()}] ---\n")
                    f.write(res.stdout)
            ref_run_executed = True
        except subprocess.TimeoutExpired:
            print("[ TIME ] Reference execution timed out.")
        except Exception as e:
            print(f"[ ERROR ] Reference execution failed: {e}")
    else:
        print(f"[ INFO ] GCC Status: {gcc_status}")
        if gcc_status == "fail" and gcc_log:
            print(f"--- GCC Log ---\n{gcc_log}")

    # ==========================================
    # --- Output Verification ---
    # ==========================================
    print("\n--- Verification Report ---")

    if llvm_run_executed and ref_run_executed:
        if filecmp.cmp(llvm_output_txt, ref_output_txt, shallow=False):
            print("[ OK ] LLVM Target Match! Our output equals GCC output.")
        else:
            print("[ FAIL ] LLVM Target Mismatch!")
            with open(ref_output_txt, "r") as rf, open(llvm_output_txt, "r") as lf:
                print(f">>> EXPECTED (GCC):\n{rf.read().strip()}")
                print(f">>> ACTUAL (LLVM):\n{lf.read().strip()}")
    elif llvm_run_executed:
        print(
            "[ WARN ] LLVM ran, but reference output was missing to check correctness."
        )

    if mips_run_executed:
        ref_for_mips = (
            ref_output_txt
            if ref_run_executed
            else (llvm_output_txt if llvm_run_executed else None)
        )
        if ref_for_mips and os.path.exists(ref_for_mips):
            if filecmp.cmp(mips_output_txt, ref_for_mips, shallow=False):
                source_label = "GCC" if ref_run_executed else "LLVM"
                print(
                    f"[ OK ] MIPS Target Match! Our output equals {source_label} output."
                )
            else:
                print("[ FAIL ] MIPS Target Mismatch!")
                with open(ref_for_mips, "r") as rf, open(mips_output_txt, "r") as mf:
                    source_label = "GCC" if ref_run_executed else "LLVM"
                    print(f">>> EXPECTED ({source_label}):\n{rf.read().strip()}")
                    print(f">>> ACTUAL (MIPS):\n{mf.read().strip()}")
        else:
            print(
                "[ WARN ] MIPS ran, but no reference target output was available to compare."
            )


if __name__ == "__main__":
    multiprocessing.freeze_support()
    if len(sys.argv) > 1:
        test_single_file(sys.argv[1])
    else:
        print("Usage: python test_single.py <path_to_c_file>")
