import re
import os

import src.issue_printer as ispr
from src.parser.AST.Node import Node


class Preprocessor:
    def __init__(self, issue_collector=None):
        self.defines = {}
        self.included_files = set()
        self.issues = issue_collector if issue_collector is not None else []

    def process_file(self, path, line: int):
        absolute_path = os.path.abspath(path)

        if absolute_path in self.included_files:
            return ""
        self.included_files.add(absolute_path)

        if not os.path.exists(absolute_path):
            dummy = Node(line, 0)
            self.issues.append(ispr.non_existant_file_error(dummy, path))
            return ""

        with open(absolute_path, "r") as f:
            lines = f.readlines()

        base_dir = os.path.dirname(absolute_path)
        return self._process_lines(lines, base_dir)

    def _process_lines(self, lines, base_dir):
        output_lines = []
        skip_stack = []

        line_nr = 0
        for line in lines:
            line_nr += 1
            stripped = line.strip()

            # --- 1. Include Guards ---
            if stripped.startswith("#ifndef"):
                parts = stripped.split()
                if len(parts) > 1:
                    macro = parts[1]
                    should_skip = (len(skip_stack) > 0 and skip_stack[-1]) or (
                        macro in self.defines
                    )
                    skip_stack.append(should_skip)
                continue

            if stripped.startswith("#endif"):
                if skip_stack:
                    skip_stack.pop()
                continue

            if skip_stack and skip_stack[-1]:
                continue

            if stripped.startswith("#define"):
                parts = stripped.split(maxsplit=2)
                macro_name = parts[1] if len(parts) >= 2 else None
                if macro_name is None or not re.match(
                    r"^[A-Za-z_][A-Za-z0-9_]*$", macro_name
                ):
                    dummy = Node(line_nr, 0)
                    self.issues.append(
                        ispr.IssuePrinter(
                            dummy,
                            f"Invalid macro name in preprocessor directive: '{stripped}'",
                            code="E127",
                        )
                    )
                    continue
                if len(parts) >= 3:
                    self.defines[macro_name] = parts[2]
                elif len(parts) == 2:
                    self.defines[macro_name] = ""
                continue

            include_match = re.match(r'#include\s+"([^"]+)"', stripped)
            if include_match:
                rel_path = include_match.group(1)
                full_path = os.path.join(base_dir, rel_path)

                included_code = self.process_file(full_path, line_nr)
                output_lines.append(included_code)
                continue

            if stripped.startswith("#include <"):
                output_lines.append(line)
                continue
            processed_line = line
            sorted_macros = sorted(self.defines.keys(), key=len, reverse=True)

            for macro in sorted_macros:
                value = self.defines[macro]
                if macro and re.search(rf"\b{re.escape(macro)}\b", processed_line):
                    processed_line = re.sub(
                        rf"\b{re.escape(macro)}\b", str(value), processed_line
                    )

            output_lines.append(processed_line)

        return "".join(output_lines)
