import os
import sys
from src.driver import PipelineConfig, CompilerPipeline


def run_ci_check(test_dir="src/tests"):
    test_dir = os.path.abspath(test_dir)
    failed_files = []
    total_count = 0

    for root, _, files in os.walk(test_dir):
        for file in files:
            if file.endswith(".c"):
                total_count += 1
                file_path = os.path.join(root, file)
                rel_path = os.path.relpath(file_path, test_dir)

                try:
                    base_name = os.path.splitext(file_path)[0]

                    config = PipelineConfig(
                        file_path=file_path,
                        ast_output=f"ast_{base_name}.dot",
                        llvmir_output=f"{base_name}.ll",
                        output_files_allowed=False,
                        print_errors=False,
                    )

                    pipeline = CompilerPipeline(config)
                    pipeline.run()

                except Exception as e:
                    failed_files.append((rel_path, str(e)))

    print("\n" + "=" * 30)
    print(f"CI Results: {total_count - len(failed_files)}/{total_count} Passed")
    print("=" * 30)

    if failed_files:
        print("\nFailures detected in the following files:")
        for path, err in failed_files:
            print(f"  - {path}")
        sys.exit(1)

    print("All files parsed successfully.")
    sys.exit(0)


if __name__ == "__main__":
    run_ci_check()
