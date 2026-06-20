import sys
import os
from src.driver import PipelineConfig, CompilerPipeline


def main():
    if len(sys.argv) > 1:
        target_file = sys.argv[1]
    else:
        target_file = "src/tests/dynamic-test.c"

    print(f"Starting compilation for: {target_file}")

    base_name = os.path.splitext(target_file)[0]

    config = PipelineConfig(
        file_path=target_file,
        ast_output=f"{base_name}.png",
        # bin_output=f"{base_name}",
        llvmir_output=f"{base_name}.ll",
    )

    pipeline = CompilerPipeline(config)
    pipeline.run()


if __name__ == "__main__":
    main()
