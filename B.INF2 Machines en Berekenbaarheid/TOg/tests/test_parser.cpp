#include "../CFG.h"
#include "../SLRParser.h"
#include "../ast/AST.h"
#include "../ast/ExpressionOperatorConfig.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace expr;

static void test_simple_addition(SLRParser& parser, expr::OperatorEnvironment env) {
    std::ostringstream       dummy;
    std::vector<std::string> tokens = {"2", "+", "3"};

    auto                     ast    = parser.parse(tokens, dummy);
    assert(ast && "AST should not be null for '2 + 3'");

    // Shape
    assert(ast->toString() == "(+ 2 3)");
    // Value
    assert(ast->evaluateWith(env) == 5);
}

static void test_precedence(SLRParser& parser, expr::OperatorEnvironment env) {
    std::ostringstream       dummy;
    std::vector<std::string> tokens = {"3", "+", "4", "*", "2"};

    auto                     ast    = parser.parse(tokens, dummy);
    assert(ast && "AST should not be null for '3 + 4 * 2'");

    assert(ast->toString() == "(+ 3 (* 4 2))");
    assert(ast->evaluateWith(env) == 11);
}

static void test_parentheses(SLRParser& parser, expr::OperatorEnvironment env) {
    std::ostringstream       dummy;
    std::vector<std::string> tokens = {"(", "3", "+", "4", ")", "*", "2"};

    auto                     ast    = parser.parse(tokens, dummy);
    assert(ast && "AST should not be null for '(3 + 4) * 2'");

    assert(ast->toString() == "(* (+ 3 4) 2)");
    assert(ast->evaluateWith(env) == 14);
}

static void test_unary_function(SLRParser& parser) {
    std::ostringstream       dummy;
    std::vector<std::string> tokens = {"cos", "(", "3", ")", "+", "1"};

    auto                     ast    = parser.parse(tokens, dummy);
    assert(ast && "AST should not be null for 'cos(3) + 1'");

    assert(ast->toString() == "(+ (cos 3) 1)");
}

static void test_special_valid_expression(SLRParser& parser) {
    std::ostringstream       dummy;
    std::vector<std::string> tokens = {"3", "+", "+", "4"};

    // 1 + is unary for the sign of 4 and the other is binary for the sum of 2 numbers
    // (+ 3 (+ 4))
    auto ast = parser.parse(tokens, dummy);
    // This should fail and return nullptr!
    assert(ast && "AST should not be null for special valid expression '3 + (+ 4)'");
}

int main() {
    std::cout << "Running SLR parser tests...\n";

    expr::OperatorEnvironment env = expr::OperatorEnvironment::createDefault();
    expr::loadBinaryOperatorsFromFile(env, "../config/operators.json");

    CFG                 cfg("../config/expression_grammar.json");
    cfg::OperatorConfig ops("../config/operators.json");

    cfg.setOperatorConfig(&ops);

    SLRParser parser(cfg);
    parser.setDebug(true);
    parser.setOperatorConfig(&ops);

    {
        std::ostringstream tableDummy;
        parser.slr(tableDummy);
    }

    test_simple_addition(parser, env);
    test_precedence(parser, env);
    test_parentheses(parser, env);
    test_unary_function(parser);
    test_special_valid_expression(parser);

    std::cout << "All SLR parser tests passed successfully!\n";
    return 0;
}
