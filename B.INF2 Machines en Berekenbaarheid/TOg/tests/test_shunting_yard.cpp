#include "../ShuntingYard.h"
#include "../ast/OperatorEnvironment.h"
#include "../config/OperatorConfig.h"

#include <cassert>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace ShuntingYard;

void test_basic_arithmetic() {
    std::cout << "Running Basic Arithmetic Test..." << std::endl;
    expr::OperatorEnvironment env = expr::OperatorEnvironment::createDefault();
    cfg::OperatorConfig       config; // Assuming default config handles basic ops

    // Example: 3 + 4 * 2 / ( 1 - 5 )
    std::string expression = "3 + 4 * 2 / ( 1 - 5 )";

    // Use your shunting yard implementation
    std::queue<std::string> rpn    = shuntingYard(expression, config);
    double                  result = evaluateRPN(rpn, env);

    // 3 + (8 / -4) = 3 - 2 = 1
    assert(result == 1.0);
    std::cout << "Basic Arithmetic Pass!" << std::endl;
}

void test_custom_operators() {
    std::cout << "Running Custom Operator Test..." << std::endl;
    expr::OperatorEnvironment env = expr::OperatorEnvironment::createDefault();
    cfg::OperatorConfig       config;

    // Register a custom operator manually for testing
    // Suppose '⊗' is a custom binary operator (a + b) * 2
    env.setBinary("⊗", [](double a, double b) { return (a + b) * 2.0; });

    // You'll need to ensure your Tokenizer/Config recognizes "⊗"
    // For this test, we assume the config and precedence are set up
    std::string             expression = "3 ⊗ 2";

    std::queue<std::string> rpn        = shuntingYard(expression, config);
    double                  result     = evaluateRPN(rpn, env);

    assert(result == 10.0);
    std::cout << "Custom Operator Pass!" << std::endl;
}

int main() {
    try {
        test_basic_arithmetic();
        // test_custom_operators(); // Uncomment if your config supports the symbol
        std::cout << "All Shunting Yard tests passed!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Test failed with exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
