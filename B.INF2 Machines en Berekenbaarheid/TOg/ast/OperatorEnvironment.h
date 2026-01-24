// ast/OperatorEnvironment.h
#ifndef AST_OPERATORENV_H
#define AST_OPERATORENV_H

#include <functional>
#include <stack>
#include <string>
#include <unordered_map>

namespace expr {

/**
 * @brief Runtime environment that stores the meaning of operators
 *
 * This allows users to redefine existing operators
 * and to introduce completely new operator symbols
 */
class OperatorEnvironment {
  public:
    using UnaryFunction  = std::function<double(double)>;
    using BinaryFunction = std::function<double(double, double)>;

    /**
     * @brief Registers or overwrites a unary operator implementation
     *
     * @param symbol Symbol for the unary operator
     * @param fn Function that implements the operator
     */
    void setUnary(const std::string& symbol, UnaryFunction fn);

    /**
     * @brief Registers or overwrites a binary operator implementation
     *
     * @param symbol Symbol for the binary operator
     * @param fn Function that implements the operator
     */
    void setBinary(const std::string& symbol, BinaryFunction fn);

    /**
     * @brief Checks whether a binary operator with the given symbol
     */
    bool hasUnary(const std::string& symbol) const;

    /**
     * @brief Checks whether a binary operator with the given symbol exists
     */
    bool hasBinary(const std::string& symbol) const;

    /**
     * @brief Applies a unary operator to a value
     *
     * @throws std::runtime_error if the operator is not defined
     */
    double applyUnary(const std::string& symbol, double value) const;

    /**
     * @brief Applies a binary operator to two operands.
     *
     * @throws std::runtime_error if the operator is not defined.
     */
    double applyBinary(const std::string& symbol, double left, double right) const;

    /**
     * @brief Creates a default environment with standard arithmetic operators
     */
    static OperatorEnvironment createDefault();

  private:
    std::unordered_map<std::string, UnaryFunction>  unaryOps_;
    std::unordered_map<std::string, BinaryFunction> binaryOps_;
};
} // namespace expr

#endif
