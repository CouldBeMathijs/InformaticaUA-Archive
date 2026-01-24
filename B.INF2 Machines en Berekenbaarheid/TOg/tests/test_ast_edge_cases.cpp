// Dit testfile is uitbreiding voor test_ast.cpp en hij test edge cases en error handling voor AST
// nodes

#include "../ast/AST.h"
#include "../ast/OperatorEnvironment.h"

#include <cassert>
#include <cmath>
#include <iostream>

using namespace expr;

// Helper functie voor evaluate() als het niet bestaat
double evaluateWithDefault(ASTNode* node) {
    static OperatorEnvironment defaultEnv = OperatorEnvironment::createDefault();
    return node->evaluateWith(defaultEnv);
}

// Test 1: Division by zero
void test_division_by_zero() {
    std::cout << "Testing division by zero... ";

    // Test regular division
    auto expr1 = makeBinary(BinaryOp::Divide, makeNumber(5), makeNumber(0));
    try {
        evaluateWithDefault(expr1.get());
        assert(false && "Should have thrown exception");
    } catch (const std::exception& e) {
        // Should throw - check error message contains "Division by zero"
        std::string msg = e.what();
        assert(msg.find("Division by zero") != std::string::npos ||
               msg.find("division") != std::string::npos);
    }

    // Test division by very small number (not zero)
    auto   expr2  = makeBinary(BinaryOp::Divide, makeNumber(5), makeNumber(0.0000001));
    double result = evaluateWithDefault(expr2.get());
    assert(result > 10000000); // 5 / 0.0000001 = 50,000,000

    std::cout << "✓" << std::endl;
}

// Test 2: Very large and very small numbers
void test_extreme_numbers() {
    std::cout << "Testing extreme numbers... ";

    // Very small
    auto tiny = makeNumber(0.000000001);
    assert(std::abs(evaluateWithDefault(tiny.get()) - 1e-9) < 1e-15);

    // Very large
    auto huge = makeNumber(1000000000);
    assert(std::abs(evaluateWithDefault(huge.get()) - 1e9) < 1);

    // Negative extreme
    auto neg = makeNumber(-0.000000001);
    assert(std::abs(evaluateWithDefault(neg.get()) + 1e-9) < 1e-15);

    // Test toString for extreme numbers doesn't crash
    std::string tinyStr = tiny->toString();
    std::string hugeStr = huge->toString();
    std::string negStr  = neg->toString();
    assert(!tinyStr.empty());
    assert(!hugeStr.empty());
    assert(!negStr.empty());

    std::cout << "✓" << std::endl;
}

// Test 3: Floating point precision in operations
void test_floating_precision() {
    std::cout << "Testing floating point precision... ";

    // 1/3 should be approximately 0.333...
    auto   oneThird = makeBinary(BinaryOp::Divide, makeNumber(1), makeNumber(3));
    double result   = evaluateWithDefault(oneThird.get());
    assert(std::abs(result - 0.3333333333333333) < 1e-12);

    // 0.1 + 0.2 should be approximately 0.3 (floating point challenge)
    auto pointThree = makeBinary(BinaryOp::Add, makeNumber(0.1), makeNumber(0.2));
    result          = evaluateWithDefault(pointThree.get());
    assert(std::abs(result - 0.3) < 1e-15);

    std::cout << "✓" << std::endl;
}

// Test 4: Deeply nested expressions
void test_deep_nesting() {
    std::cout << "Testing deep nesting... ";

    // Build: ((((1 + 1) + 1) + 1) ... + 1)  - 10 levels deep
    std::unique_ptr<ASTNode> expr = makeNumber(1);
    for (int i = 0; i < 9; i++)
        expr = makeBinary(BinaryOp::Add, std::move(expr), makeNumber(1));

    // Should evaluate to 10
    assert(std::abs(evaluateWithDefault(expr.get()) - 10) < 0.001);

    // toString should work without crashing
    std::string str = expr->toString();
    assert(!str.empty());
    assert(str.length() > 10); // Should be a reasonably long string

    std::cout << "✓ (10 levels deep)" << std::endl;
}

// Test 5: Operator error cases
void test_custom_operator_errors() {
    std::cout << "Testing custom operator errors... ";

    OperatorEnvironment env = OperatorEnvironment::createDefault();

    // Test 1: Unknown unary operator should throw
    auto unknownUnary = makeCustomUnary("unknown_op", makeNumber(5));
    try {
        unknownUnary->evaluateWith(env);
        assert(false && "Should have thrown for unknown unary operator");
    } catch (const std::exception& e) {
        std::string msg = e.what();
        assert(msg.find("Unknown") != std::string::npos ||
               msg.find("unknown") != std::string::npos);
    }

    // Test 2: Unknown binary operator should throw
    auto unknownBinary = makeCustomBinary("unknown_op", makeNumber(2), makeNumber(3));
    try {
        unknownBinary->evaluateWith(env);
        assert(false && "Should have thrown for unknown binary operator");
    } catch (const std::exception& e) {
        std::string msg = e.what();
        assert(msg.find("Unknown") != std::string::npos ||
               msg.find("unknown") != std::string::npos);
    }

    // Test 3: Custom operator without environment should throw
    auto customOp = makeCustomUnary("$", makeNumber(5));

    // Gebruik een LEEG environment (zonder $ operator)
    OperatorEnvironment emptyEnv; // Niet createDefault()!

    try {
        customOp->evaluateWith(emptyEnv);
        assert(false && "Should have thrown for unknown operator");
    } catch (const std::exception& e) {
        std::string msg = e.what();
        // Accepteer verschillende error messages
        bool hasError = (msg.find("Unknown") != std::string::npos ||
                         msg.find("unknown") != std::string::npos ||
                         msg.find("$") != std::string::npos);
        assert(hasError && "Should mention unknown operator");
    }

    std::cout << "✓" << std::endl;
}

// Test 6: Operator overriding behavior
void test_operator_override_consistency() {
    std::cout << "Testing operator override consistency... ";

    OperatorEnvironment env = OperatorEnvironment::createDefault();

    // Override "+" to do multiplication instead
    env.setBinary("+", [](double a, double b) { return a * b; });

    // Create expression using builtin Add (which should now multiply)
    auto expr = makeBinary(BinaryOp::Add, makeNumber(3), makeNumber(4));

    // Should use overridden operator: 3 * 4 = 12
    assert(expr->evaluateWith(env) == 12);

    // Regular evaluation (with default env) should still add: 3 + 4 = 7
    assert(evaluateWithDefault(expr.get()) == 7);

    std::cout << "✓" << std::endl;
}

// Test 7: Complex expression with all operators
void test_all_operators_together() {
    std::cout << "Testing all operators together... ";

    // Expression: -((2 + 3) * 4) / (5 - 1)
    auto expr =
        makeBinary(BinaryOp::Divide,
                   makeUnary(UnaryOp::Minus,
                             makeBinary(BinaryOp::Multiply,
                                        makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3)),
                                        makeNumber(4))),
                   makeBinary(BinaryOp::Subtract, makeNumber(5), makeNumber(1)));

    // -((2+3)*4) / (5-1) = -20 / 4 = -5
    double result = evaluateWithDefault(expr.get());
    assert(std::abs(result - (-5)) < 0.001);

    // Check string representation
    std::string str = expr->toString();
    assert(str.find("/") != std::string::npos);
    assert(str.find("*") != std::string::npos);
    assert(str.find("+") != std::string::npos);
    assert(str.find("-") != std::string::npos);

    // Test prettyPrint doesn't crash
    std::string pretty = expr->prettyPrint();
    assert(!pretty.empty());

    std::cout << "✓" << std::endl;
}

// Test 8: Unary operator edge cases
void test_unary_edge_cases() {
    std::cout << "Testing unary operator edge cases... ";

    // Unary plus on zero
    auto unaryPlusZero = makeUnary(UnaryOp::Plus, makeNumber(0));
    assert(evaluateWithDefault(unaryPlusZero.get()) == 0);

    // Unary minus on zero
    auto unaryMinusZero = makeUnary(UnaryOp::Minus, makeNumber(0));
    assert(evaluateWithDefault(unaryMinusZero.get()) == 0);

    // Double negative
    auto doubleNeg = makeUnary(UnaryOp::Minus, makeUnary(UnaryOp::Minus, makeNumber(7)));
    assert(evaluateWithDefault(doubleNeg.get()) == 7);

    // Unary on negative number
    auto unaryOnNeg = makeUnary(UnaryOp::Minus, makeNumber(-3));
    assert(evaluateWithDefault(unaryOnNeg.get()) == 3);

    std::cout << "✓" << std::endl;
}

int main() {
    std::cout << "Running AST EDGE CASE tests" << std::endl;

    test_division_by_zero();
    test_extreme_numbers();
    test_floating_precision();
    test_deep_nesting();
    test_custom_operator_errors();
    test_operator_override_consistency();
    test_all_operators_together();
    test_unary_edge_cases();

    std::cout << "ALL AST EDGE CASE TESTS PASSED!" << std::endl;

    return 0;
}
