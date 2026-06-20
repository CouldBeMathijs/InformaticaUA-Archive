import os
import re
import subprocess
import src.mips_target.mips_builtins as builtins


class MIPSGenerator:
    def __init__(self, llvm_content, print_info):
        self.llvm_content = llvm_content
        self.print_info = print_info
        self.module = ""

    def generate(self):
        temp_ll = "temp_mips.ll"
        temp_s = "temp_mips.s"

        success = False

        with open(temp_ll, "w") as f:
            f.write(str(self.llvm_content))

        try:
            cmd = [
                "clang",
                "--target=mipsel-unknown-unknown-elf",
                "-march=mips32",
                "-mno-abicalls",
                "-fno-PIC",
                "-G",
                "0",
                "-fno-builtin",
                "-O0",
                "-S",
                temp_ll,
                "-o",
                temp_s,
            ]
            if self.print_info:
                print(f"[ INFO ] Running clang for MIPS: {' '.join(cmd)}")

            result = subprocess.run(cmd, capture_output=True, text=True)
            if result.returncode != 0:
                print(f"[ ERROR ] clang MIPS generation failed:\n{result.stderr}")
                return False

            with open(temp_s, "r") as f:
                asm_content = f.read()

            processed_asm = self.post_process(asm_content)

            self.module = processed_asm

            success = True
            return True

        finally:
            if success:
                if os.path.exists(temp_ll):
                    os.remove(temp_ll)
                if os.path.exists(temp_s):
                    os.remove(temp_s)
            else:
                print(
                    f"[ DEBUG ] Keeping temp files for inspection, something went wrong: {temp_ll}, {temp_s}"
                )

    def post_process(self, asm_content):
        """
        Adjusts the assembly for MARS/SPIM compatibility.
        """
        import struct

        asm_content = re.sub(r"\$1\b", "$25", asm_content)

        # Fix labels that begin with $
        asm_content = asm_content.replace("$str", "_str")
        asm_content = asm_content.replace("$func", "_func")
        asm_content = asm_content.replace("$CPI", "_CPI")
        asm_content = asm_content.replace("$BB", "BB")

        asm_content = re.sub(
            r"teq\s+\$[a-z0-9]+,\s+\$[a-z0-9]+,\s+\d+", "# teq removed", asm_content
        )

        asm_content = re.sub(
            r"lui\s+(\$[a-z0-9]+),\s+%hi\(([^)]+)\)", r"la\t\1, \2", asm_content
        )

        def replace_lo(match):
            inner_content = match.group(1)  # Everything inside %lo(...)
            # Look for explicit algebraic pointer additions/subtractions like label+4 or label-8
            offset_match = re.search(r"([-+]\s*\d+)", inner_content)
            if offset_match:
                # Returns the extracted arithmetic offset (e.g. "+4")
                return offset_match.group(1).replace(" ", "")
            return "0"

        asm_content = re.sub(r"%lo\(([^)]+)\)", replace_lo, asm_content)

        asm_content = asm_content.replace(".4byte", ".word")
        asm_content = asm_content.replace(".2byte", ".half")
        asm_content = asm_content.replace(".1byte", ".byte")
        asm_content = asm_content.replace(".asciz", ".asciiz")
        asm_content = asm_content.replace(".bss", ".data")

        lines = asm_content.splitlines()
        text_lines = []
        data_lines = []

        # Removed '.text' and '.data' from block deletion list
        to_delete = [
            ".section",
            ".type",
            ".size",
            ".ident",
            ".addrsig",
            ".p2align",
            ".nan",
            ".module",
            ".file",
            ".cfi_",
            ".mask",
            ".fmask",
            ".bss",
        ]

        current_label = ""

        # Uninitialized memory directives (.space, .zero, .comm)
        data_keywords = [
            ".asciiz",
            ".word",
            ".half",
            ".byte",
            ".8byte",
            ".space",
            ".zero",
            ".comm",
        ]

        for line in lines:
            stripped = line.strip()

            # Skip explicit structural segments from Clang so we can group them cleanly
            if stripped in [".text", ".data"]:
                continue

            if not stripped or any(pattern in stripped for pattern in to_delete):
                continue

            # Catch data labels and global directives accumulatively
            if stripped.endswith(":") or stripped.startswith(".globl"):
                if current_label:
                    current_label += "\n" + line
                else:
                    current_label = line
                continue

            # Route initialization declarations & allocations into data segment arrays
            if any(k in stripped for k in data_keywords):
                if current_label:
                    # FIX: Force 4-byte word boundary alignment right before data declarations
                    data_lines.append("    .align 2")
                    data_lines.append(current_label)
                    current_label = ""

                if ".8byte" in stripped:
                    raw_val = stripped.split()[-1]
                    if "#" in raw_val:
                        raw_val = raw_val.split("#")[0].strip()

                    try:
                        if raw_val.lower().startswith("0x"):
                            val = int(raw_val, 16)
                        else:
                            float_val = float(raw_val)
                            val = struct.unpack("<Q", struct.pack("<d", float_val))[0]

                        low_32 = val & 0xFFFFFFFF
                        high_32 = (val >> 32) & 0xFFFFFFFF
                        data_lines.append(
                            f"    .word 0x{low_32:08x}, 0x{high_32:08x} # converted double pattern"
                        )
                    except ValueError as e:
                        data_lines.append(
                            f"    # Failed to parse double literal '{raw_val}': {e}"
                        )
                elif ".asciiz" in stripped:
                    match = re.search(r'\.asciiz\s+"(.*)"', line)
                    if match:
                        raw_str = match.group(1)
                        bytes_list = []
                        i = 0
                        while i < len(raw_str):
                            if raw_str[i] == "\\":
                                # Check the next character
                                if i + 1 < len(raw_str):
                                    next_char = raw_str[i + 1]
                                    if next_char == "\\":
                                        bytes_list.append(92)  # Literal backslash
                                        i += 2
                                    elif next_char == "n":
                                        bytes_list.append(10)  # Literal newline
                                        i += 2
                                    elif next_char == "t":
                                        bytes_list.append(9)  # Literal tab
                                        i += 2
                                    elif next_char == '"':
                                        bytes_list.append(34)  # Literal double quote
                                        i += 2
                                    else:
                                        # It's an escaped letter we treat as a literal backslash + char
                                        bytes_list.append(92)
                                        i += 1
                                else:
                                    bytes_list.append(92)
                                    i += 1
                            else:
                                bytes_list.append(ord(raw_str[i]))
                                i += 1

                        # Add a null-terminator (0) byte at the end
                        bytes_list.append(0)

                        # Convert into a safe .byte sequence
                        bytes_str = ", ".join(str(b) for b in bytes_list)
                        data_lines.append(
                            f"    .byte {bytes_str} # Safely preserved string literal"
                        )
                    else:
                        data_lines.append(line)
                else:
                    data_lines.append(line)
            else:
                # Code execution instructions land here safely in the text array
                if current_label:
                    text_lines.append(current_label)
                    current_label = ""
                text_lines.append(line)

        final_asm = []

        # Emit the data segment blocks first
        if data_lines:
            final_asm.append(".data")
            final_asm.extend(data_lines)
            final_asm.append("")

        # Emit text segment with explicit globl entry linkage
        final_asm.append(".text")
        final_asm.append(".globl main")
        final_asm.append(".globl printf")

        fixed_text_lines = []
        is_in_main = False
        for line in text_lines:
            if line.strip().startswith("main:"):
                is_in_main = True

            if is_in_main and "jr" in line and "$ra" in line:
                fixed_text_lines.append("    li $v0, 10          # Exit syscall")
                fixed_text_lines.append("    syscall")
                continue

            if (
                is_in_main
                and line.strip() == "nop"
                and fixed_text_lines
                and "syscall" in fixed_text_lines[-1]
            ):
                continue

            if ".end" in line and "main" in line:
                is_in_main = False

            fixed_text_lines.append(line)
            if "main:" in line:
                fixed_text_lines.append("    li $25, -8")
                fixed_text_lines.append("    and $sp, $sp, $25")

        final_asm.extend(fixed_text_lines)

        # Append standard built-in simulator function definitions
        final_asm.append("\n# --- Custom Implementation of printf for SPIM ---\n")
        final_asm.append(builtins.get_printf_asm())
        final_asm.append("\n# --- Custom Implementation of scanf for SPIM ---\n")
        final_asm.append(builtins.get_scanf_asm())
        final_asm.append(
            "\n# --- Custom Implementation of Lib Functions for SPIM ---\n"
        )
        final_asm.append(builtins.get_lib_asm())

        # Clang might generate 'movf' or 'movt' which SPIM doesn't support
        processed_asm = []
        label_counter = 0
        for line in final_asm:
            if "movf" in line:
                parts = line.split()
                if len(parts) >= 3:
                    rd = parts[1].strip(",")
                    rs = parts[2].strip(",")
                    label = f"L_mov_fix_{label_counter}"
                    label_counter += 1
                    processed_asm.append(f"    bc1t {label}")
                    processed_asm.append("    nop")
                    processed_asm.append(f"    move {rd}, {rs}")
                    processed_asm.append(f"{label}:")
                else:
                    processed_asm.append(line)
            elif "movt" in line:
                parts = line.split()
                if len(parts) >= 3:
                    rd = parts[1].strip(",")
                    rs = parts[2].strip(",")
                    label = f"L_mov_fix_{label_counter}"
                    label_counter += 1
                    processed_asm.append(f"    bc1f {label}")
                    processed_asm.append("    nop")
                    processed_asm.append(f"    move {rd}, {rs}")
                    processed_asm.append(f"{label}:")
                else:
                    processed_asm.append(line)
            else:
                processed_asm.append(line)

        return "\n".join(processed_asm)
