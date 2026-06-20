import sys
import os
import subprocess
import shutil
import tempfile
from copy import deepcopy
from dataclasses import dataclass
from typing import Optional

from antlr4 import FileStream, CommonTokenStream, InputStream
from src.llvm_target.ir_generator import IRGenerator
from src.parser.AST.ConstantPropagator import ConstantPropagator
from src.parser.AST.ConstantFolder import ConstantFolder
from src.parser.AST.DeadCodeEliminator import DeadCodeEliminator
from src.parser.grammars.GrammarLexer import GrammarLexer
from src.parser.grammars.GrammarParser import GrammarParser
from src.parser.AST.Builder import Builder
from src.parser.AST.SymbolTableBuilder import SymbolTableBuilder
from src.parser.AST.Visualizer import VisualizerVisitor
from src.parser.CustomAntlrErrorListener import CustomAntlrErrorListener
from src.parser.Preprocessor import Preprocessor
from src.mips_target.mips_generator import MIPSGenerator


@dataclass
class PipelineConfig:
    """Slaat alle instellingen voor de compiler pipeline op."""

    file_path: str
    constant_folding: bool = True
    constant_propagation: bool = True
    print_errors: bool = True
    ast_output: Optional[str] = None
    llvmir_output: Optional[str] = None
    mips_output: Optional[str] = None
    save_exec_output: Optional[str] = None
    output_files_allowed: bool = True
    bin_output: Optional[str] = None
    print_info: bool = True
    preprocessing: bool = True
    print_symboltable: bool = False
    symboltable_output: Optional[str] = None
    declarations_allowed_in_for_loops: bool = False


class CompilerPipeline:
    def __init__(self, config: PipelineConfig):
        self.config = config
        self.all_issues = []
        self.ast = None

    def _check_dependencies(self):
        deps = ["lli", "clang", "gcc"]
        missing_deps = [dep for dep in deps if not shutil.which(dep)]
        if missing_deps:
            raise Exception(f"Missing following dependencies {missing_deps}")

    def _handle_constant_optimization(self):
        cfg = self.config
        if not cfg.constant_folding and not cfg.constant_propagation:
            return

        dce = DeadCodeEliminator()

        if cfg.constant_folding and not cfg.constant_propagation:
            self.ast = ConstantFolder().visit(self.ast)
            self.ast = dce.optimize(self.ast)
            return

        if cfg.constant_propagation and not cfg.constant_folding:
            self.ast = ConstantPropagator(issue_collector=self.all_issues).visit(
                self.ast
            )
            self.ast = dce.optimize(self.ast)
            return

        # Beide aan: voer uit tot er geen veranderingen meer zijn
        propagator = ConstantPropagator(issue_collector=self.all_issues)
        folder = ConstantFolder()
        while True:
            self.ast = propagator.visit(self.ast)
            self.ast = folder.visit(self.ast)
            self.ast = dce.optimize(self.ast)
            if not propagator.changed and not folder.changed and not dce.changed:
                break
            propagator.changed = False
            folder.changed = False
            dce.changed = False

    def _has_fatal_errors(self) -> bool:
        return any(issue.severity == "Error" for issue in self.all_issues)

    def _print_report(self):
        if not self.config.print_errors:
            return
        if not self.all_issues:
            print("\n[ SUCCESS ] Compilation successful: No issues found.")
            return
        for issue in self.all_issues:
            print(issue, file=sys.stderr)

    def run(self):
        cfg = self.config
        try:
            self._check_dependencies()

            if not os.path.exists(cfg.file_path):
                print(f"[ ERROR ] File not found: {cfg.file_path}", file=sys.stderr)
                sys.exit(1)
            # ...

            # --- Lexing & Parsing ---
            if cfg.preprocessing:
                prep = Preprocessor(self.all_issues)
                preprocessed_code = prep.process_file(cfg.file_path, 0)
                input_stream = InputStream(preprocessed_code)
            else:
                input_stream = FileStream(cfg.file_path, encoding="utf-8")

            lexer = GrammarLexer(input_stream)
            stream = CommonTokenStream(lexer)
            parser = GrammarParser(stream)

            error_listener = CustomAntlrErrorListener(file_name=cfg.file_path)
            error_listener.issues = self.all_issues
            lexer.removeErrorListeners()
            lexer.addErrorListener(error_listener)
            parser.removeErrorListeners()
            parser.addErrorListener(error_listener)

            tree = parser.root()

            if self._has_fatal_errors():
                self._print_report()
                sys.exit(1)

            # --- AST & Symbol Table ---
            self.ast = Builder(
                stream, self.config.declarations_allowed_in_for_loops, self.all_issues
            ).visit(tree)
            table_build = SymbolTableBuilder(issue_collector=self.all_issues)
            table_build.visit(self.ast)

            if cfg.print_symboltable:
                print(table_build.symtab)
            if cfg.symboltable_output and cfg.output_files_allowed:
                if cfg.print_info:
                    print(
                        f"[ INFO ] Writing symbol table to {cfg.symboltable_output}..."
                    )
                with open(cfg.symboltable_output, "w") as f:
                    f.write(table_build.symtab.to_dot())

            if self._has_fatal_errors():
                self._print_report()
                sys.exit(1)

            # --- Optimization ---
            self._handle_constant_optimization()

            # --- LLVM IR Generation ---
            if cfg.llvmir_output or cfg.bin_output:
                generator = IRGenerator()
                generator.visit(deepcopy(self.ast))

                ll_path_for_clang = cfg.llvmir_output
                temp_ll_file = None

                if cfg.llvmir_output and cfg.output_files_allowed:
                    if cfg.print_info:
                        print(f"[ INFO ] Writing LLVM IR to {cfg.llvmir_output}...")
                    with open(cfg.llvmir_output, "w") as f:
                        f.write(str(generator.module))

                # If binary output is requested without an explicit LLVM output path,
                # create a temporary IR file for clang.
                if cfg.bin_output and ll_path_for_clang is None:
                    temp_ll_file = tempfile.NamedTemporaryFile(
                        mode="w", suffix=".ll", delete=False
                    )
                    temp_ll_file.write(str(generator.module))
                    temp_ll_file.flush()
                    temp_ll_file.close()
                    ll_path_for_clang = temp_ll_file.name

                # --- Binary Compilation ---
                if cfg.bin_output and ll_path_for_clang:
                    exec_cmd = [
                        "clang",
                        ll_path_for_clang,
                        "-Wno-int-conversion",
                        "-o",
                        cfg.bin_output,
                    ]
                    if cfg.print_info:
                        print(f"[ INFO ] Compiling executable: {' '.join(exec_cmd)}")

                    try:
                        result = subprocess.run(
                            exec_cmd, capture_output=True, text=True
                        )
                        if result.returncode != 0:
                            print(
                                f"[ ERROR ] Clang compilation failed:\n{result.stderr}"
                            )
                        elif cfg.save_exec_output:
                            bin_path = cfg.bin_output
                            if os.name != "nt" and "/" not in bin_path:
                                bin_path = f"./{bin_path}"

                            if cfg.print_info:
                                print(f"[ INFO ] Executing: {bin_path}\n" + "-" * 40)

                            run_result = subprocess.run(
                                [bin_path], capture_output=True, text=True
                            )
                            with open(cfg.save_exec_output, "w") as f:
                                f.write(run_result.stdout)

                            if cfg.print_info:
                                print("-" * 40)
                                print(
                                    f"[ INFO ] Process exited with code {run_result.returncode}"
                                )
                    except FileNotFoundError:
                        print(
                            "[ ERROR ] Clang not found. Make sure it is installed and in your PATH."
                        )
                    finally:
                        if temp_ll_file is not None and os.path.exists(
                            temp_ll_file.name
                        ):
                            os.remove(temp_ll_file.name)

            # --- MIPS ---
            if cfg.mips_output:
                llvm_mips_generator = IRGenerator("mipsel-unknown-unknown-elf")
                llvm_mips_generator.visit(deepcopy(self.ast))

                mips_generator = MIPSGenerator(
                    llvm_mips_generator.module, cfg.print_info
                )
                if mips_generator.generate():
                    with open(cfg.mips_output, "w") as f:
                        f.write(str(mips_generator.module))

            # --- Visualization ---
            if cfg.ast_output:
                visualizer = VisualizerVisitor()
                visualizer.visit(self.ast)
                if cfg.output_files_allowed:
                    print(f"[ INFO ] Rendering AST image to {cfg.ast_output}...")
                    visualizer.render(f"{cfg.ast_output}")

            self._print_report()

        except Exception as e:
            e.add_note(f"File which caused error: {cfg.file_path}")
            raise e
