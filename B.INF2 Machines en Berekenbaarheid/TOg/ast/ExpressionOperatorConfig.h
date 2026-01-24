#ifndef MBTOG_OPERATORCONFIG_H
#define MBTOG_OPERATORCONFIG_H

#include "../external/json.hpp"
#include "OperatorEnvironment.h"

#include <memory>
#include <string>

namespace expr {

/**
 * @brief Internal representation of a parsed operator expression.
 *
 * An OpExpr instance represents a small expression tree used to define
 * the behavior of a custom unary or binary operator. These expression
 * trees are read from JSON and converted into executable lambdas that
 * are stored inside the OperatorEnvironment.
 */
struct OpExpr {
    /**
     * @brief Enumerates the supported node kinds inside an OpExpr tree.
     */
    enum class Kind { Number, Var, Unary, Binary };

    Kind                    kind;

    double                  value = 0.0;

    std::string             varName;

    std::string             unaryOp;
    std::unique_ptr<OpExpr> arg;

    std::string             binaryOp;
    std::unique_ptr<OpExpr> left;
    std::unique_ptr<OpExpr> right;
};

/**
 * @brief Parses a JSON object representing an operator expression into an OpExpr tree.
 *
 * @param j JSON node representing a part of an operator definition.
 * @return A newly allocated OpExpr tree matching the JSON structure.
 * @throws std::runtime_error if the JSON contains an unknown or invalid node type.
 */
std::unique_ptr<OpExpr> parseOpExpr(const nlohmann::json& j);

/**
 * @brief Evaluates an OpExpr tree representing a custom *binary* operator.
 * @param e The expression tree.
 * @param a The value bound to variable "a".
 * @param b The value bound to variable "b".
 * @return Computed result of the expression.
 * @throws std::runtime_error if unknown variable names or operators occur.
 */
double evalOpExpr(const OpExpr& e, double a, double b);

/**
 * @brief Loads all custom unary and binary operators from a JSON file.
 *
 * @param env OperatorEnvironment receiving the parsed operator implementations.
 * @param filename Path to the JSON configuration file.
 *
 * @throws std::runtime_error if the file cannot be opened or if the JSON structure is invalid.
 */
void loadBinaryOperatorsFromFile(OperatorEnvironment& env, const std::string& filename);
} // namespace expr

#endif
