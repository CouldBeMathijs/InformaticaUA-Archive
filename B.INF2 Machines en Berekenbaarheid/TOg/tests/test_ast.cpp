#include "../ast/AST.h"
#include "../ast/ExpressionOperatorConfig.h"
#include "../ast/OperatorEnvironment.h"

#include <cassert>
#include <iostream>

using namespace expr;

// Helper functie die evaluateWith gebruikt met default environment
double evaluateWithDefault(ASTNode* node) {
    static OperatorEnvironment defaultEnv = OperatorEnvironment::createDefault();
    return node->evaluateWith(defaultEnv);
}

void test_number_node() {
    auto n = makeNumber(42);
    assert(evaluateWithDefault(n.get()) == 42);
}

void test_addition() {
    auto expr = makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3));
    assert(evaluateWithDefault(expr.get()) == 5);
}

void test_complex_expression() {
    // (2 + 3) * 4 - 5  = 15
    auto exprAst = makeBinary(BinaryOp::Subtract,
                              makeBinary(BinaryOp::Multiply,
                                         makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3)),
                                         makeNumber(4)),
                              makeNumber(5));

    assert(evaluateWithDefault(exprAst.get()) == 15);
}

void test_unary_plus() {
    auto expr = makeUnary(UnaryOp::Plus, makeNumber(5));
    assert(evaluateWithDefault(expr.get()) == 5);
}

void test_unary_minus() {
    auto expr = makeUnary(UnaryOp::Minus, makeNumber(5));
    assert(evaluateWithDefault(expr.get()) == -5);
}

void test_nested_unary() {
    // --5 == 5
    auto expr = makeUnary(UnaryOp::Minus, makeUnary(UnaryOp::Minus, makeNumber(5)));
    assert(evaluateWithDefault(expr.get()) == 5);
}

void test_unary_with_binary() {
    // -(2 + 3) * -4  == -5 * -4 == 20
    auto expr = makeBinary(
        BinaryOp::Multiply,
        makeUnary(UnaryOp::Minus, makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3))),
        makeUnary(UnaryOp::Minus, makeNumber(4)));
    assert(evaluateWithDefault(expr.get()) == 20);
}

void test_unary_precedence() {
    // unary binds tighter: -2 * 3 == (-2) * 3 == -6
    auto expr =
        makeBinary(BinaryOp::Multiply, makeUnary(UnaryOp::Minus, makeNumber(2)), makeNumber(3));
    assert(evaluateWithDefault(expr.get()) == -6);
}

void test_toString_number() {
    auto n = makeNumber(5);
    assert(n->toString() == "5");
}

void test_toString_unary() {
    auto n = makeUnary(UnaryOp::Minus, makeNumber(5));
    assert(n->toString() == "(- 5)");
}

void test_toString_binary() {
    auto expr = makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3));
    assert(expr->toString() == "(+ 2 3)");
}

void test_toString_nested() {
    // (- (* (+ 2 3) 4))
    auto expr = makeUnary(UnaryOp::Minus,
                          makeBinary(BinaryOp::Multiply,
                                     makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3)),
                                     makeNumber(4)));

    assert(expr->toString() == "(- (* (+ 2 3) 4))");
}

void test_toString_float_cleanup() {
    auto n = makeNumber(5.000000);
    assert(n->toString() == "5");
}

void test_toString_decimal() {
    auto n = makeNumber(3.500000);
    assert(n->toString() == "3.5");
}

void test_prettyPrint_number() {
    auto        n        = makeNumber(7);
    std::string expected = "Number(7)\n";
    assert(n->prettyPrint() == expected);
}

void test_prettyPrint_unary() {
    auto        expr     = makeUnary(UnaryOp::Minus, makeNumber(5));

    std::string expected = "UnaryOp(-)\n"
                           "  Number(5)\n";

    assert(expr->prettyPrint() == expected);
}

void test_prettyPrint_binary_simple() {
    auto        expr     = makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3));

    std::string expected = "BinaryOp(+)\n"
                           "  Number(2)\n"
                           "  Number(3)\n";

    assert(expr->prettyPrint() == expected);
}

void test_prettyPrint_nested() {
    // (- (* (+ 2 3) 4))
    auto        expr     = makeUnary(UnaryOp::Minus,
                                     makeBinary(BinaryOp::Multiply,
                                                makeBinary(BinaryOp::Add, makeNumber(2), makeNumber(3)),
                                                makeNumber(4)));

    std::string expected = "UnaryOp(-)\n"
                           "  BinaryOp(*)\n"
                           "    BinaryOp(+)\n"
                           "      Number(2)\n"
                           "      Number(3)\n"
                           "    Number(4)\n";

    assert(expr->prettyPrint() == expected);
}

void test_custom_unary_operator() {
    OperatorEnvironment env = OperatorEnvironment::createDefault();
    env.setUnary("!", [](double v) { return v == 0 ? 1 : 0; });

    auto expr = makeCustomUnary("!", makeNumber(0));
    assert(expr->evaluateWith(env) == 1);

    auto expr2 = makeCustomUnary("!", makeNumber(5));
    assert(expr2->evaluateWith(env) == 0);
}

void test_custom_binary_operator() {
    OperatorEnvironment env = OperatorEnvironment::createDefault();
    env.setBinary("⊗", [](double a, double b) { return a * b + 1; });

    auto expr = makeCustomBinary("⊗", makeNumber(3), makeNumber(4));
    assert(expr->evaluateWith(env) == 13); // 3*4+1
}

void test_override_builtin_operator() {
    OperatorEnvironment env = OperatorEnvironment::createDefault();

    // Redefine "+" so that it subtracts.
    env.setBinary("+", [](double a, double b) { return a - b; });

    auto expr = makeBinary(BinaryOp::Add, makeNumber(10), makeNumber(3));

    // Now 10 + 3 should become 10 - 3 = 7
    assert(expr->evaluateWith(env) == 7);
}

void test_nested_custom_and_builtin_mix() {
    OperatorEnvironment env = OperatorEnvironment::createDefault();

    env.setBinary("⊗", [](double a, double b) { return (a + b) * 2; });

    // (2 ⊗ 3) + 4

    auto expr = makeBinary(BinaryOp::Add, makeCustomBinary("⊗", makeNumber(2), makeNumber(3)),
                           makeNumber(4));

    // ⊗ → (2+3)*2 = 10 → 10 + 4 = 14
    assert(expr->evaluateWith(env) == 14);
}

// Test dat custom operators zonder environment een fout geven
void test_custom_operator_without_env() {
    auto customOp = makeCustomUnary("$", makeNumber(5));

    // Gebruik een lege environment (zonder operators)
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
}

// Test dat built-in operators werken met evaluateWith
void test_builtin_with_evaluateWith() {
    OperatorEnvironment env  = OperatorEnvironment::createDefault();

    auto                expr = makeBinary(BinaryOp::Multiply, makeNumber(3), makeNumber(4));
    assert(expr->evaluateWith(env) == 12);

    auto expr2 = makeUnary(UnaryOp::Minus, makeNumber(7));
    assert(expr2->evaluateWith(env) == -7);
}

int main() {
    std::cout << "Running AST tests...\n";

    test_number_node();
    test_addition();
    test_complex_expression();

    test_unary_plus();
    test_unary_minus();
    test_nested_unary();
    test_unary_with_binary();
    test_unary_precedence();

    test_toString_number();
    test_toString_unary();
    test_toString_binary();
    test_toString_nested();

    test_toString_float_cleanup();
    test_toString_decimal();

    test_prettyPrint_number();
    test_prettyPrint_unary();
    test_prettyPrint_binary_simple();
    test_prettyPrint_nested();

    test_custom_unary_operator();
    test_custom_binary_operator();
    test_override_builtin_operator();
    test_nested_custom_and_builtin_mix();

    test_custom_operator_without_env();
    test_builtin_with_evaluateWith();

    std::cout << "All AST tests passed successfully!\n";
    return 0;
}
