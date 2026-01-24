#include "../SLRParser.h"
#include "../ast/ExpressionOperatorConfig.h"

#include <fstream>
#include <iostream>

// test to see that the new parser works

int main() {
    std::string input = R"###(
{
  "Variables": ["S", "E", "T", "F", "G"],
  "Terminals": ["OP_BIN", "OP_UN", "FUNC1", "FUNC2", "(", ")", ",", "NUM", "ID"],
  "Start": "S",
  "Productions": [
    { "head": "S", "body": ["E"] },

    { "head": "E", "body": ["E", "OP_BIN", "T"] },
    { "head": "E", "body": ["T"] },

    { "head": "T", "body": ["OP_UN", "T"] },
    { "head": "T", "body": ["F"] },

    { "head": "F", "body": ["G"] },

    { "head": "G", "body": ["FUNC1", "(", "E", ")"] },
    { "head": "G", "body": ["FUNC2", "(", "E", ",", "E", ")"] },
    { "head": "G", "body": ["(", "E", ")"] },
    { "head": "G", "body": ["NUM"] },
    { "head": "G", "body": ["ID"] }
  ]
}

)###";
    try {
        CFG                 cfg(input, false);
        cfg::OperatorConfig ops("../config/operators.json");
        cfg.setOperatorConfig(&ops);

        expr::OperatorEnvironment env = expr::OperatorEnvironment::createDefault();
        expr::loadBinaryOperatorsFromFile(env, "../config/operators.json");

        // 2. Build SLR parser
        SLRParser parser(cfg);
        parser.setDebug(true);
        parser.setOperatorConfig(&ops);

        // dump SLR table to a file to see states/actions
        {
            std::ofstream tableOut("slr_table.txt");
            parser.slr(tableOut);
        }

        auto runTest = [&](const std::string& name, const std::vector<std::string>& tokens,
                           bool tryEvaluate = true) {
            std::cout << "\n==== Test: " << name << " ====\n";
            std::cout << "Tokens: ";
            for (const auto& t : tokens)
                std::cout << "'" << t << "' ";
            std::cout << "\n";

            // 3. Parse tokens into AST
            auto ast = parser.parse(tokens, std::cout);
            if (!ast) {
                std::cout << "Parse FAILED (AST == nullptr)\n";
                return;
            }

            // 4. Show AST structure
            std::cout << "AST toString(): " << ast->toString() << "\n";
            std::cout << "AST prettyPrint():\n" << ast->prettyPrint("") << "\n";

            // 5. Evaluate if requested (and safe)
            if (tryEvaluate) {
                try {
                    double result = ast->evaluateWith(env); // no OperatorEnvironment
                    std::cout << "evaluateWith(env) = " << result << "\n";
                } catch (const std::exception& e) {
                    std::cout << "evaluateWith(env) threw: " << e.what() << "\n";
                }
            }
        };

        // ---------------------------
        // TEST 1: 3 + 4 * 2  (precedence)
        // Expected AST: (+ 3 (* 4 2))
        // Expected evaluateWith(env) = 11
        // ---------------------------
        runTest("3 + 4 * 2", {"3", "+", "4", "*", "2"});

        // ---------------------------
        // TEST 2: (3 + 4) * 2  (parentheses)
        // Expected AST: (* (+ 3 4) 2)
        // Expected evaluateWith(env) = 14
        // ---------------------------
        runTest("(3 + 4) * 2", {"(", "3", "+", "4", ")", "*", "2"});

        // ---------------------------
        // TEST 3: cos(3) + 1  (built-in func)
        // This will parse and build AST with CustomUnaryOpNode("cos", ...)
        // BUT evaluateWith(env) will probably throw because CustomUnaryOpNode::evaluateWith(env)
        // needs an OperatorEnvironment. So we set tryEvaluate = false here.
        // ---------------------------
        runTest("cos(3) + 1", {"cos", "(", "3", ")", "+", "1"},
                /*tryEvaluate=*/false);

        // ---------------------------
        // TEST 4: x + 3  (identifier)
        // Should parse and produce an AST: (+ x 3)
        // evaluateWith(env) will throw (IdentifierNode not implemented yet),
        // so again: tryEvaluate = false.
        // ---------------------------
        runTest("x + 3", {"x", "+", "3"},
                /*tryEvaluate=*/false);

        // ---------------------------
        // TEST 5: malformed expression to see error handling
        // Example: "3 + + 4" -> should fail parse and return nullptr
        // ---------------------------
        runTest("3 + + 4 (expected parse error)", {"3", "+", "+", "4"});

        std::cout << "\nAll tests done.\n";
    } catch (const std::exception& e) {
        std::cerr << "Fatal error in main: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
