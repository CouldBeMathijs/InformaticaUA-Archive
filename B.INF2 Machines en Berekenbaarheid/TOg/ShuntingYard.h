#ifndef MBTOG_SHUNTING_YARD_H
#define MBTOG_SHUNTING_YARD_H

#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace cfg {
class OperatorConfig;
}

namespace expr {
class OperatorEnvironment;
}

/**
 * @brief Namespace containing utilities for converting infix expressions to RPN
 *        using the Shunting Yard algorithm, and evaluating RPN expressions.
 *
 * This module typically:
 * - Tokenizes / reads an input expression
 * - Uses operator precedence and associativity rules to output RPN (Reverse Polish Notation)
 * - Evaluates the RPN queue with an operator environment (operators/functions/variables)
 */
namespace ShuntingYard {

/**
 * @brief Operator metadata used by the Shunting Yard algorithm.
 *
 * precedence: higher value = binds stronger (evaluates earlier)
 * rightAssociative: true for right-associative ops (e.g., exponentiation ^)
 */
struct OpInfo {
    /// Precedence level (higher means higher priority)
    int precedence;

    /// Whether the operator is right-associative (e.g. ^)
    bool rightAssociative;
};

/**
 * @brief Default precedence table for common arithmetic operators.
 *
 * Note: This is a static mapping used as a baseline. If you support more operators
 * via cfg::OperatorConfig, your implementation may override/extend these rules.
 */
static const std::unordered_map<std::string, OpInfo> PRECEDENCE = {
    {"^", {4, true}}, {"*", {3, false}}, {"/", {3, false}}, {"+", {2, false}}, {"-", {2, false}}};

/**
 * @brief Convert an infix expression string into RPN (Reverse Polish Notation)
 *        using the Shunting Yard algorithm.
 *
 * This function reads the input expression, applies precedence/associativity rules,
 * and returns a queue representing the equivalent RPN sequence.
 *
 * Operator definitions / supported tokens are typically guided by ops (OperatorConfig),
 * for example to support custom operators, unary operators, functions, etc.
 *
 * @param input The original infix expression as a string (e.g., "3 + 4 * 2").
 * @param ops Operator configuration describing valid operators (and possibly functions).
 * @return A queue of tokens in RPN order (postfix notation).
 */
std::queue<std::string> shuntingYard(const std::string& input, const cfg::OperatorConfig& ops);

/**
 * @brief Evaluate a queue of RPN tokens and return the computed numeric result.
 *
 * The evaluation uses a stack-based approach:
 * - Numbers are pushed onto the stack
 * - Operators pop their operands and push the result
 *
 * The OperatorEnvironment typically provides the actual implementation of operators
 * (and optionally variables/constants/functions).
 *
 * @param rpn Queue of RPN tokens to evaluate. It is passed by reference and
 *            is usually consumed (emptied) during evaluation.
 * @param env Operator environment providing operator semantics (e.g., +, -, *, /, ^).
 * @return The resulting value after evaluating the full RPN expression.
 */
double evaluateRPN(std::queue<std::string>& rpn, const expr::OperatorEnvironment& env);

} // namespace ShuntingYard

#endif
