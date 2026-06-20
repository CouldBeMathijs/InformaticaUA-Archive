# Compilers B.INF2

Developed by: Cnackle, CouldBeMathijs & Nils Van de Velde


---

## Getting Started

### Prerequisites

- **Python 3.8+**
- **LLVM (lli, clang)**
- **MIPS Simulator (spim)**
- **GCC** (for reference testing)

```bash
pip install -r requirements.txt
```

### Usage

Run the compiler through the main entry point:

```bash
python3 -m src.main --input <source_file.c> [options]
```

**Common Options:**

- `--target_bin <file>`: Compile to a native binary
- `--target_mips <file.mips>`: Compile to MIPS assembly
- `--target_llvm <file.ll>`: Generate LLVM IR
- `--render_ast <file.dot>`: Generate AST visualization (Graphviz)
- `--render_symb <file.dot>`: Render the symbol table
- `--no-optimizations`: Disable constant folding/propagation

---

## Testing & Demonstration

### Video Demonstration Script

For the May 30th deadline, we have prepared a script that demonstrates the compiler's ability to generate working MIPS and Native Binary code for a variety of features.

```bash
python3 run_video_tests.py
```

### Verification against GCC

To verify our compiler's output against GCC for any C file:

```bash
python3 -m src.test_single <path_to_c_file>
```

---

## Features (Deadline May 30th)

### Core Functionality

- **MIPS & Binary Backends**: Full support for code generation to MIPS assembly and native executables.
- **Control Flow**: If-else, While, and For loops (including declarations).
- **Types & Pointers**: Integer, Float, Char, and multi-level pointers with arithmetic.
- **Functions**: Full support for definitions, declarations, and recursion.

### Advanced & Extra Features

- **Optimizations**: Scope-aware Constant Folding and Propagation. Unused variable elimination and dead-code elimination for conditionals that evaluate to false.
- **Structs & Unions**: Support for nested structures, union types, and arrays containing structures.
- **Memory Management**:
  - **Heap Allocation**: `malloc` support for Dynamic Arrays and Dynamic Structs.
  - **Strings**: Heap-allocated string support.
- **Function Overloading**: Support for multiple functions with different parameter types.
- **File I/O**: Integrated file reading and writing capabilities.
- **Preprocessor**: Support for `#include` and macros.
- **Typedef & Sizeof**: Standard compliant implementation of `typedef` and the `sizeof` function.
- **Character Escapes**: Support for complex characters using the `'\x'` notation (e.g., `\n`, `\u45`, `\x1A`).

---

## Test Info & Known Edge Cases

The following test cases track specific behaviors, limitations, or instances of undefined behavior verified against GCC:

- **[Arraytest 59](src/tests/test_set_3/MipsTests/ArrayTests/test59.c)**: `-Wint-conversion` makes it so this test succeeds in GCC, while it really should fail.
- **[Calculation test 1 MIPS](src/tests/test_set_3/MipsTests/CalculationTests/test1.c)**: Undefined behavior (negative bitshift).
- **[Calculation test 3 MIPS](src/tests/test_set_3/MipsTests/CalculationTests/test3.c)**: Undefined behavior (negative bitshift).
- **[Conversiontest 16](src/tests/test_set_3/ASTTests/ConversionTests/test16.c)**: Printing a float using `%d` is undefined.
- **[Function test 51 LLVM](src/tests/test_set_3/LLVMTests/FunctionTests/test51.c)**: Undefined behavior (dangling pointer).
- **[Function test 51 MIPS](src/tests/test_set_3/MipsTests/FunctionTests/test51.c)**: Undefined behavior (dangling pointer) (added const qualifier warning).
- **[Functiontest 12](src/tests/test_set_3/MipsTests/FunctionTests/test12.c)**: `-Wint-conversion` makes it so this test succeeds in GCC, while it really should fail.
- **[Functiontest 20](src/tests/test_set_3/MipsTests/FunctionTests/test20.c)**: GCC passes but with undefined behavior. We expect our compiler to fail.
- **[Printtest 16](src/tests/test_set_3/LLVMTests/PrintTests/test16.c)**: Printing a pointer will not always give the same result.
- **[Scantests 8](src/tests/test_set_3/LLVMTests/ScanTests/test8.c)**: Scanf-test containing more than 4 inputs is not supported by the test script.
- **[Printtest 10](src/tests/test_set_3/MipsTests/PrintTests/test10.c)**: Printing a float literal as a pointer will not always give the same result.
- **[FileIOtest 7](src/tests/test_set_3/MipsTests/FileIOTests/test7.c)**: Undefined behavior when printing a non-terminated string.
- **[FIleIOtest 8](src/LLVMTests/FileIOTests/test8.c)**: Undefined behavior when printing a non-terminated string.
- **[PrintTest 45](src/tests/test_set_3/MipsTests/PrintTests/test45.c)**: Undefined behavior due to uninitialized string pointer.
- **[PrintTest 15](src/tests/test_set_3/LLVMTests/PrintTests/test15.c)**: Undefined behavior.
- **[Tests1 36](src/tests/test_set_1/test_file_36.c)**: Undefined behavior, self initialization.
