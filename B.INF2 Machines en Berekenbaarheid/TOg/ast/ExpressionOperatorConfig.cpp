#include "ExpressionOperatorConfig.h"

#include "OperatorEnvironment.h"

#include <cmath>
#include <fstream>
#include <memory>
#include <stdexcept>

using json = nlohmann::json;

namespace expr {

/* ========================================================================== */
/*                              JSON → OpExpr                                 */
/* ========================================================================== */

std::unique_ptr<OpExpr> parseOpExpr(const json& j) {
    auto              expr = std::make_unique<OpExpr>();

    const std::string type = j.at("type").get<std::string>();

    if (type == "number") {
        expr->kind  = OpExpr::Kind::Number;
        expr->value = j.at("value").get<double>();
    } else if (type == "var") {
        expr->kind    = OpExpr::Kind::Var;
        expr->varName = j.at("name").get<std::string>();
    } else if (type == "unary") {
        expr->kind    = OpExpr::Kind::Unary;
        expr->unaryOp = j.at("op").get<std::string>();
        expr->arg     = parseOpExpr(j.at("arg"));
    } else if (type == "binary") {
        expr->kind     = OpExpr::Kind::Binary;
        expr->binaryOp = j.at("op").get<std::string>();
        expr->left     = parseOpExpr(j.at("left"));
        expr->right    = parseOpExpr(j.at("right"));
    } else {
        throw std::runtime_error("Unknown op-expr type in JSON: " + type);
    }

    return expr;
}

/* ========================================================================== */
/*                       Supported cmath Unary Operators                      */
/* ========================================================================== */

static double applyUnaryOp(const std::string& op, double v) {
    if (op == "abs" || op == "fabs")
        return std::fabs(v);

    if (op == "sin")
        return std::sin(v);
    if (op == "cos")
        return std::cos(v);
    if (op == "tan")
        return std::tan(v);
    if (op == "asin")
        return std::asin(v);
    if (op == "acos")
        return std::acos(v);
    if (op == "atan")
        return std::atan(v);

    if (op == "sinh")
        return std::sinh(v);
    if (op == "cosh")
        return std::cosh(v);
    if (op == "tanh")
        return std::tanh(v);

    if (op == "exp")
        return std::exp(v);
    if (op == "log")
        return std::log(v);
    if (op == "log10")
        return std::log10(v);
    if (op == "sqrt")
        return std::sqrt(v);

    if (op == "ceil")
        return std::ceil(v);
    if (op == "floor")
        return std::floor(v);
    if (op == "trunc")
        return std::trunc(v);
    if (op == "round")
        return std::round(v);
    if (op == "neg")
        return -v;
    if (op == "pos")
        return v;

    throw std::runtime_error("Unknown unary op in op-expr: " + op);
}

/* ========================================================================== */
/*                       Supported cmath Binary Operators                     */
/* ========================================================================== */

static double applyBinaryOp(const std::string& op, double a, double b) {
    if (op == "add")
        return a + b;
    if (op == "sub")
        return a - b;
    if (op == "mul")
        return a * b;
    if (op == "div")
        return a / b;

    if (op == "pow")
        return std::pow(a, b);
    if (op == "fmod")
        return std::fmod(a, b);
    if (op == "atan2")
        return std::atan2(a, b);

    if (op == "min")
        return std::fmin(a, b);
    if (op == "max")
        return std::fmax(a, b);

    throw std::runtime_error("Unknown binary op in op-expr: " + op);
}

/* ========================================================================== */
/*             Generic evaluation engine used for unary & binary trees        */
/* ========================================================================== */

template <typename GetVar> static double evalOpExprGeneric(const OpExpr& e, const GetVar& getVar) {
    switch (e.kind) {
        case OpExpr::Kind::Number:
            return e.value;

        case OpExpr::Kind::Var:
            return getVar(e.varName);

        case OpExpr::Kind::Unary: {
            double v = evalOpExprGeneric(*e.arg, getVar);
            return applyUnaryOp(e.unaryOp, v);
        }

        case OpExpr::Kind::Binary: {
            double lv = evalOpExprGeneric(*e.left, getVar);
            double rv = evalOpExprGeneric(*e.right, getVar);
            return applyBinaryOp(e.binaryOp, lv, rv);
        }
    }
    throw std::runtime_error("Invalid OpExpr kind");
}

/* ========================================================================== */
/*                Public evaluator for custom *binary* operator bodies        */
/* ========================================================================== */

double evalOpExpr(const OpExpr& e, double a, double b) {
    auto getVar = [a, b](const std::string& name) -> double {
        if (name == "a")
            return a;
        if (name == "b")
            return b;
        throw std::runtime_error("Unknown var name in binary op-expr: " + name);
    };
    return evalOpExprGeneric(e, getVar);
}

/* ========================================================================== */
/*                Internal evaluator for custom *unary* operator bodies       */
/* ========================================================================== */

static double evalOpExprUnary(const OpExpr& e, double x) {
    auto getVar = [x](const std::string& name) -> double {
        if (name == "x")
            return x;
        throw std::runtime_error("Unknown var name in unary op-expr: " + name);
    };
    return evalOpExprGeneric(e, getVar);
}

/* ========================================================================== */
/*                  JSON loader helpers: binary & unary operator blocks       */
/* ========================================================================== */

static void loadBinaryOperatorsFromJson(OperatorEnvironment& env, const json& root) {
    if (!root.contains("binary_operators"))
        return;

    const auto& arr = root.at("binary_operators");
    if (!arr.is_array())
        throw std::runtime_error("\"binary_operators\" must be an array");

    for (const auto& def : arr) {
        const std::string symbol     = def.at("symbol").get<std::string>();
        const json&       exprJson   = def.at("expr");

        auto              exprTree   = parseOpExpr(exprJson);
        auto              sharedTree = std::shared_ptr<OpExpr>(std::move(exprTree));

        env.setBinary(symbol, [sharedTree](double a, double b) -> double {
            return evalOpExpr(*sharedTree, a, b);
        });
    }
}

static void loadUnaryOperatorsFromJson(OperatorEnvironment& env, const json& root) {
    if (!root.contains("unary_operators"))
        return;

    const auto& arr = root.at("unary_operators");
    if (!arr.is_array())
        throw std::runtime_error("\"unary_operators\" must be an array");

    for (const auto& def : arr) {
        const std::string symbol     = def.at("symbol").get<std::string>();
        const json&       exprJson   = def.at("expr");

        auto              exprTree   = parseOpExpr(exprJson);
        auto              sharedTree = std::shared_ptr<OpExpr>(std::move(exprTree));

        env.setUnary(symbol,
                     [sharedTree](double x) -> double { return evalOpExprUnary(*sharedTree, x); });
    }
}

/* ========================================================================== */
/*                             Public JSON file loader                        */
/* ========================================================================== */

void loadBinaryOperatorsFromFile(OperatorEnvironment& env, const std::string& filename) {
    std::ifstream in(filename);
    if (!in)
        throw std::runtime_error("Could not open operator config file: " + filename);

    json root;
    in >> root;

    loadBinaryOperatorsFromJson(env, root);
    loadUnaryOperatorsFromJson(env, root);
}
} // namespace expr
