import argparse
from src.driver import PipelineConfig, CompilerPipeline


def main():
    argparser = argparse.ArgumentParser(description="C Compiler")

    argparser.add_argument(
        "-input",
        "--input",
        required=True,
        dest="input",
        help="The input file (e.g input_file.c)",
    )
    argparser.add_argument(
        "-render_ast",
        "--render_ast",
        dest="render_ast",
        help="Generate the AST and save to given output file (e.g ast_output.dot)",
    )
    argparser.add_argument(
        "-render_symb",
        "--render_symb",
        dest="render_symb",
        help="Render the symbol table to the given output file (e.g symb_output.dot)",
    )
    argparser.add_argument(
        "-target_llvm",
        "--target_llvm",
        dest="target_llvm",
        help="Compile to LLVM IR and save to given output file (e.g output_file.ll)",
    )
    argparser.add_argument(
        "-target_mips",
        "--target_mips",
        dest="target_mips",
        help="Compile to a MIPS-32 binary.",
    )
    argparser.add_argument(
        "-target_bin",
        "--target_bin",
        dest="target_bin",
        help="Compile to a native binary and save to given output file (e.g output_file)",
    )
    argparser.add_argument(
        "-no-optimizations",
        "--no-optimizations",
        dest="no_optimizations",
        action="store_true",
        help="Turn off compiler optimizations",
    )
    argparser.add_argument(
        "-no-preprocessing",
        "--no-preprocessing",
        dest="no_preprocessing",
        action="store_true",
        help="Turn off the preprocessor",
    )
    argparser.add_argument(
        "-save_exec_output",
        "--save_exec_output",
        dest="save_exec_output",
        help="Save the execution output of the binary to the given file",
    )
    argparser.add_argument(
        "-print_symb",
        "--print_symb",
        dest="print_symb",
        action="store_true",
        help="Print the symbol table to stdout",
    )
    argparser.add_argument(
        "-no-errors",
        "--no-errors",
        dest="no_errors",
        action="store_true",
        help="Turn off error message printing",
    )
    argparser.add_argument(
        "-no-info",
        "--no-info",
        dest="no_info",
        action="store_true",
        help="Turn off info message printing",
    )
    argparser.add_argument(
        "-no-output-files",
        "--no-output-files",
        dest="no_output_files",
        action="store_true",
        help="Disable the creation of all output files",
    )

    args = argparser.parse_args()
    input_file = args.input

    print(f"Starting compilation for: {input_file}")

    # Build the configuration using the new PipelineConfig dataclass
    config = PipelineConfig(
        file_path=input_file,
        constant_folding=(not args.no_optimizations),
        constant_propagation=(not args.no_optimizations),
        print_errors=(not args.no_errors),
        ast_output=args.render_ast,
        llvmir_output=args.target_llvm,
        mips_output=args.target_mips,
        save_exec_output=args.save_exec_output,
        output_files_allowed=(not args.no_output_files),
        bin_output=args.target_bin,
        print_info=(not args.no_info),
        preprocessing=(not args.no_preprocessing),
        print_symboltable=(args.print_symb),
        symboltable_output=args.render_symb,
    )

    pipeline = CompilerPipeline(config)
    pipeline.run()


if __name__ == "__main__":
    main()
