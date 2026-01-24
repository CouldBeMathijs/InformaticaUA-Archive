/**
 * @file OperatorEnvironment.cpp
 * @brief Implementation of the operator environment for configurable operators.
 */

#include "OperatorEnvironment.h"

#include <stdexcept>

namespace expr {

void OperatorEnvironment::setUnary(const std::string&                       symbol,
                                   expr::OperatorEnvironment::UnaryFunction fn) {
    unaryOps_[symbol] = std::move(fn);
}

void OperatorEnvironment::setBinary(const std::string& symbol, BinaryFunction fn) {
    binaryOps_[symbol] = std::move(fn);
}

bool OperatorEnvironment::hasUnary(const std::string& symbol) const {
    return unaryOps_.contains(symbol);
}

bool OperatorEnvironment::hasBinary(const std::string& symbol) const {
    return binaryOps_.contains(symbol);
}

double OperatorEnvironment::applyUnary(const std::string& symbol, double value) const {
    const auto it = unaryOps_.find(symbol);
    if (it == unaryOps_.end())
        throw std::runtime_error("Unknown unary operator: " + symbol);
    return it->second(value);
}

double OperatorEnvironment::applyBinary(const std::string& symbol, double left,
                                        double right) const {
    auto it = binaryOps_.find(symbol);
    if (it == binaryOps_.end())
        throw std::runtime_error("Unknown binary operator: " + symbol);
    return it->second(left, right);
}

OperatorEnvironment OperatorEnvironment::createDefault() {
    OperatorEnvironment env;

    env.setUnary("+", [](double v) { return v; });
    env.setUnary("-", [](double v) { return -v; });

    env.setBinary("+", [](double a, double b) { return a + b; });
    env.setBinary("-", [](double a, double b) { return a - b; });
    env.setBinary("*", [](double a, double b) { return a * b; });
    env.setBinary("/", [](double a, double b) {
        if (b == 0.0)
            throw std::runtime_error("Division by zero in OperatorEnvironment");
        return a / b;
    });

    return env;
}
} // namespace expr
