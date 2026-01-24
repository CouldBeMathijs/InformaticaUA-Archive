# Mini Expression Evaluator

**MBTOg Project - University of Antwerp**
*An advanced mathematical evaluator based on formal languages.*

---

## The Team
* **Norkella**
* **Adriaan De Kaey**
* **CouldBeMathijs**
* **Ania**

---

## Project Overview
This project translates the abstract theory from the **Machines and Computability** course into a functional application. The tool is capable of tokenizing complex mathematical expressions (Tokenizer.cpp), validating them using formal methods (PDA & CYK), converting them to an Abstract Syntax Tree (AST) using an SLR parser, and finally computing them (AST & Shunting Yard algorithm).

### Key Features:
- **3-Step Validation**: PDA (structure), CYK (grammar), and SLR Parser (semantics).
- **Zero-Code Updates**: No hard-coded logic; the language rules, operators, and automata are loaded completely dynamically via JSON.
- **Interactive GUI**: Real-time visualization of the PDA automata and the generated AST trees.
- **Reliability**: Computation using both AST traversal and the Shunting Yard algorithm (RPN) for maximum reliability.

---

## The Flow

The program follows the following flow:

1. **Lexical Analysis (Tokenizer):** Converts text into typed symbols (CykSymbols).
2. **Structural Validation (PDA):** A stack-based automaton checks the balance of parentheses and the base structure. 
3. **Grammatical Validation (CYK):** Checks whether the input conforms to the defined grammar using Chomsky Normal Form (CNF).
4. **Syntactic Analysis (SLR Parser):** Builds an Abstract Syntax Tree (AST) based on operator precedence and associativity.
5. **Computation:** AST, Shunting Yard.

---

## Configuration & Flexibility

The core of this project's flexibility lies in its decoupled JSON architecture. This allows us to modify the entire application's behavior without recompiling the C++ code.


- `pda_config.json`: Defines the PDA's states, alphabet, and transitions. 
- `expression_grammar.json`: Contains the Context-Free Grammar (CFG) of our mathematical language.
- `operators.json`: Defines symbols, weights (precedence), associativity, and the semantics of each operator.

---
## GUI Functionality
- **Evaluate**: Triggers the full validation and calculation chain.
- **PDA Diagram:** Generates a visual overview of the currently loaded PDA structure.
- **PDA Validate:** Displays the specific path the automaton took for the current input.
- **AST Image:** Visualizes the hierarchical structure of the mathematical expression.

---
## Testing & Quality Assurance

To ensure reliability, we performed the following checks:

- **Basic logic Verification:** We confirmed that the Tokenizer, PDA, and CYK engines strictly follow the formal methods from the course.

- **Edge Case Testing:** We tested it against complex expressions and intentional errors (like nested parentheses and invalid grammar) to ensure the validation chain holds.

- **Visual Debugging:** Using Graphviz, we manually inspected the PDA paths and AST structures to verify that math rules, such as precedence, were applied correctly.

