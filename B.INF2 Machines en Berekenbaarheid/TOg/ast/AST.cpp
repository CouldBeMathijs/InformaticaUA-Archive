/**
 * @file AST.cpp
 * @brief Implementation of AST node classes for expression evaluation
 */

#include "AST.h"

#include "OperatorEnvironment.h"

#include <cmath>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <unordered_set>

#ifdef _WIN32
constexpr std::string DOT_PATH = "dot";
#else
constexpr std::basic_string DOT_PATH = "dot";
#endif

namespace {
using namespace expr;

/**
 * @brief Converts a floating-point number to a clean string
 */
std::string cleanNumberToString(double value) {
    std::ostringstream oss;
    oss << std::setprecision(15) << std::noshowpoint << std::defaultfloat << value;

    std::string s = oss.str();

    if (s.find('e') != std::string::npos || s.find('E') != std::string::npos)
        return s;

    if (s.find('.') != std::string::npos) {
        while (!s.empty() && s.back() == '0')
            s.pop_back();

        if (!s.empty() && s.back() == '.')
            s.pop_back();
    }

    return s;
}

/**
 * @brief Maps a UnaryOp enum to its canonical symbol string.
 */
std::string unaryOpToSymbol(UnaryOp op) {
    switch (op) {
        case UnaryOp::Plus:
            return "+";
        case UnaryOp::Minus:
            return "-";
    }
    return "?";
}

/**
 * @brief Maps a BinaryOp enum to its canonical symbol string.
 */
std::string binaryOpToSymbol(BinaryOp op) {
    switch (op) {
        case BinaryOp::Add:
            return "+";
        case BinaryOp::Subtract:
            return "-";
        case BinaryOp::Multiply:
            return "*";
        case BinaryOp::Divide:
            return "/";
    }
    return "?";
}
} // namespace

namespace expr {

/**
 * @brief Constructs a GroupNode that wraps a child expression.
 */
GroupNode::GroupNode(std::unique_ptr<ASTNode> child)
    : child_(std::move(child)) {
    if (!child_)
        throw std::invalid_argument("GroupNode child cannot be null");
}

/**
 * @brief Constructs a UnaryOpNode.
 *
 * @param op Unary operator to apply.
 * @param child Subtree on which the operator is applied.
 */
UnaryOpNode::UnaryOpNode(UnaryOp op, std::unique_ptr<ASTNode> child)
    : op_(op)
    , child_(std::move(child)) {}

/**
 * @brief Constructs a BinaryOpNode with the given operator and child subtrees
 *
 * @param op The binary operator
 * @param left Unique pointer to the left operand subtree
 * @param right Unique pointer to the right operand subtree
 */
BinaryOpNode::BinaryOpNode(BinaryOp op, std::unique_ptr<ASTNode> left,
                           std::unique_ptr<ASTNode> right)
    : op_(op)
    , left_(std::move(left))
    , right_(std::move(right)) {}

/**
 * @brief Constructs a CustomUnaryOpNode.
 */
CustomUnaryOpNode::CustomUnaryOpNode(std::string symbol, std::unique_ptr<ASTNode> child)
    : symbol_(std::move(symbol))
    , child_(std::move(child)) {}

/**
 * @brief Constructs a CustomBinaryOpNode.
 */
CustomBinaryOpNode::CustomBinaryOpNode(std::string symbol, std::unique_ptr<ASTNode> left,
                                       std::unique_ptr<ASTNode> right)
    : symbol_(std::move(symbol))
    , left_(std::move(left))
    , right_(std::move(right)) {}

/**
 * @brief String representation for debugging / GUI / tests.
 *
 * Transparent: we keep canonical AST strings stable by returning the child's
 * representation without adding extra parentheses.
 */
std::string GroupNode::toString() const { return child_ ? child_->toString() : "?"; }

/**
 * @brief Returns a minimal string representation of the stored number.
 */
std::string NumberNode::toString() const { return cleanNumberToString(value_); }

/**
 * @brief String representation for debugging / GUI / tests.
 */
std::string UnaryOpNode::toString() const {
    std::string opStr = (op_ == UnaryOp::Plus ? "+" : "-");
    return "(" + opStr + " " + child_->toString() + ")";
}

/**
 * @brief String representation of this binary expression.
 */
std::string BinaryOpNode::toString() const {
    std::string opStr;
    switch (op_) {
        case BinaryOp::Add:
            opStr = "+";
            break;
        case BinaryOp::Subtract:
            opStr = "-";
            break;
        case BinaryOp::Multiply:
            opStr = "*";
            break;
        case BinaryOp::Divide:
            opStr = "/";
            break;
    }

    return "(" + opStr + " " + left_->toString() + " " + right_->toString() + ")";
}

/**
 * @brief String representation for debugging / GUI / tests.
 */
std::string CustomUnaryOpNode::toString() const {
    return "(" + symbol_ + " " + child_->toString() + ")";
}

/**
 * @brief String representation of this binary expression.
 */
std::string CustomBinaryOpNode::toString() const {
    return "(" + symbol_ + " " + left_->toString() + " " + right_->toString() + ")";
}

/**
 * @brief Pretty-prints this AST subtree in a multi-line tree layout.
 *
 * Shows a "Group" node to make parentheses visible during debugging.
 */
std::string GroupNode::prettyPrint(const std::string& indent) const {
    std::string out = indent + "Group\n";
    if (child_)
        out += child_->prettyPrint(indent + "  ");
    return out;
}

/**
 * @brief Pretty-print for NumberNode.
 */
std::string NumberNode::prettyPrint(const std::string& indent) const {
    return indent + "Number(" + cleanNumberToString(value_) + ")\n";
}

/**
 * @brief Pretty-print for UnaryOpNode.
 */
std::string UnaryOpNode::prettyPrint(const std::string& indent) const {
    std::string opStr = (op_ == UnaryOp::Plus ? "+" : "-");

    std::string out   = indent + "UnaryOp(" + opStr + ")\n";
    out += child_->prettyPrint(indent + "  ");
    return out;
}

/**
 * @brief Pretty-print for BinaryOpNode.
 */
std::string BinaryOpNode::prettyPrint(const std::string& indent) const {
    std::string opStr;
    switch (op_) {
        case BinaryOp::Add:
            opStr = "+";
            break;
        case BinaryOp::Subtract:
            opStr = "-";
            break;
        case BinaryOp::Multiply:
            opStr = "*";
            break;
        case BinaryOp::Divide:
            opStr = "/";
            break;
    }

    std::string out = indent + "BinaryOp(" + opStr + ")\n";
    out += left_->prettyPrint(indent + "  ");
    out += right_->prettyPrint(indent + "  ");
    return out;
}

std::string CustomUnaryOpNode::prettyPrint(const std::string& indent) const {
    std::string out = indent + "CustomUnaryOp(" + symbol_ + ")\n";
    out += child_->prettyPrint(indent + "  ");
    return out;
}

std::string CustomBinaryOpNode::prettyPrint(const std::string& indent) const {
    std::string out = indent + "CustomBinaryOp(" + symbol_ + ")\n";
    out += left_->prettyPrint(indent + "  ");
    out += right_->prettyPrint(indent + "  ");
    return out;
}

/**
 * @brief Evaluates the grouped sub-expression (same as evaluating its child).
 */
double GroupNode::evaluateWith(const OperatorEnvironment& env) const {
    return child_->evaluateWith(env);
}

/**
 * @brief Evaluates this number using the given operator environment.
 */
double NumberNode::evaluateWith(const OperatorEnvironment& /*env*/) const { return value_; }

/**
 * @brief Evaluates the unary operator using a configurable environment.
 */
double UnaryOpNode::evaluateWith(const OperatorEnvironment& env) const {
    const double value = child_->evaluateWith(env);

    if (const std::string symbol = unaryOpToSymbol(op_); env.hasUnary(symbol))
        return env.applyUnary(symbol, value);

    switch (op_) {
        case UnaryOp::Plus:
            return value;
        case UnaryOp::Minus:
            return -value;
    }

    throw std::runtime_error("Unknown unary operator in AST evaluation");
}

/**
 * @brief Evaluates the binary operation using a configurable environment.
 *
 * If the operator is present in the environment, that definition is used,
 * otherwise the built-in semantics are applied.
 */
double BinaryOpNode::evaluateWith(const OperatorEnvironment& env) const {
    const double l = left_->evaluateWith(env);
    const double r = right_->evaluateWith(env);

    if (const std::string symbol = binaryOpToSymbol(op_); env.hasBinary(symbol))
        return env.applyBinary(symbol, l, r);

    switch (op_) {
        case BinaryOp::Add:
            return l + r;
        case BinaryOp::Subtract:
            return l - r;
        case BinaryOp::Multiply:
            return l * r;
        case BinaryOp::Divide:
            if (r == 0.0)
                throw std::runtime_error("Division by zero in AST evaluation");
            return l / r;
        default:
            throw std::runtime_error("Unknown binary operator in AST evaluation");
    }
}

/**
 * @brief Evaluates the custom unary operator using the given environment.
 */
double CustomUnaryOpNode::evaluateWith(const OperatorEnvironment& env) const {
    double value = child_->evaluateWith(env);
    return env.applyUnary(symbol_, value);
}

/**
 * @brief Evaluates the custom binary operator using the given environment.
 */
double CustomBinaryOpNode::evaluateWith(const OperatorEnvironment& env) const {
    double l = left_->evaluateWith(env);
    double r = right_->evaluateWith(env);
    return env.applyBinary(symbol_, l, r);
}

/**
 * @brief DOT graph generator for GroupNode.
 */
int GroupNode::toDot(std::ostream& out, int& currentId) const {
    int myId = currentId++;
    out << "  node" << myId << " [label=\"( )\"];\n";

    if (child_) {
        int childId = child_->toDot(out, currentId);
        out << "  node" << myId << " -> node" << childId << ";\n";
    }

    return myId;
}

int CustomUnaryOpNode::toDot(std::ostream& out, int& currentId) const {
    const int myId = currentId++;
    out << "  node" << myId << " [label=\"" << symbol_
        << "\", style=filled, fillcolor=\"#332222\"];\n";

    if (child_) {
        const int childId = child_->toDot(out, currentId);
        out << "  node" << myId << " -> node" << childId << ";\n";
    }
    return myId;
}

int CustomBinaryOpNode::toDot(std::ostream& out, int& currentId) const {
    const int myId = currentId++;
    out << "  node" << myId << " [label=\"" << symbol_
        << "\", style=filled, fillcolor=\"#332222\"];\n";

    if (left_) {
        const int leftId = left_->toDot(out, currentId);
        out << "  node" << myId << " -> node" << leftId << ";\n";
    }
    if (right_) {
        const int rightId = right_->toDot(out, currentId);
        out << "  node" << myId << " -> node" << rightId << ";\n";
    }
    return myId;
}

int NumberNode::toDot(std::ostream& out, int& currentId) const {
    int myId = currentId++;
    // Remove trailing zeros for cleaner display
    std::string label = std::to_string(value_);
    label.erase(label.find_last_not_of('0') + 1, std::string::npos);
    if (label.back() == '.')
        label.pop_back();

    out << "  node" << myId << " [label=\"" << label
        << "\", shape=ellipse, color=\"#4CAF50\", fontcolor=\"#4CAF50\"];\n";
    return myId;
}

int BinaryOpNode::toDot(std::ostream& out, int& currentId) const {
    int         myId = currentId++;
    std::string symbol;
    switch (op_) {
        case BinaryOp::Add:
            symbol = "+";
            break;
        case BinaryOp::Subtract:
            symbol = "-";
            break;
        case BinaryOp::Multiply:
            symbol = "*";
            break;
        case BinaryOp::Divide:
            symbol = "/";
            break;
    }

    out << "  node" << myId << " [label=\"" << symbol << "\"];\n";

    if (left_) {
        int leftId = left_->toDot(out, currentId);
        out << "  node" << myId << " -> node" << leftId << ";\n";
    }
    if (right_) {
        int rightId = right_->toDot(out, currentId);
        out << "  node" << myId << " -> node" << rightId << ";\n";
    }
    return myId;
}

void ASTNode::exportToImage(const std::string& filename) const {
    std::filesystem::path outPath(filename);
    std::string           extension = outPath.extension().string();
    // Remove the dot from extension if present (e.g., ".png" -> "png")
    if (!extension.empty() && extension[0] == '.')
        extension.erase(0, 1);

    static const std::unordered_set<std::string> supported = {"png",  "jpg", "jpeg",
                                                              "webp", "svg", "pdf"};
    if (!supported.contains(extension))
        throw std::invalid_argument("Unsupported extension: " + extension);

    std::stringstream dot;
    dot << "digraph AST {\n";
    dot << "  bgcolor=\"#0c0d10\";\n";
    dot << "  fontcolor=white;\n";
    dot << "  ordering=out;\n"; // Maintains child order (left vs right)
    dot << "  ranksep=0.5;\n";
    dot << "  nodesep=0.5;\n";

    // Default node style
    dot << "  node [shape=circle, style=filled, fillcolor=\"#141414\", color=lightgray, "
           "fontcolor=white, "
           "penwidth=1.5];\n";
    // Default edge style
    dot << "  edge [color=lightgray, fontcolor=white, penwidth=1.2];\n";

    int nodeIdCounter = 0;
    this->toDot(dot, nodeIdCounter);

    dot << "}\n";

    // Temporary file creation
    std::string baseName    = outPath.stem().string();
    std::string dotFilename = "temp_ast_" + baseName + ".dot";

    {
        std::ofstream ofs(dotFilename);
        if (!ofs)
            throw std::runtime_error("Failed to write temporary dot file.");
        ofs << dot.str();
    } // Close file via RAII

    std::cout << dotFilename << " generated successfully" << std::endl;

    // Optional DPI/Size settings for raster images
    std::string settings;
    if (extension == "png" || extension == "jpg" || extension == "webp")
        settings = " -Gdpi=300 ";

    // Construct command
    std::string command = std::string(DOT_PATH) + " -T" + extension + settings + " \"" +
                          dotFilename + "\" -o \"" + filename + "\"";

    std::cout << "Executing: " + command << std::endl;

    if (int result = std::system(command.c_str()); result != 0) {
        std::cerr << "DOT command failed. Check that Graphviz is installed and path is correct.\n";
        // We do not delete the dot file here to allow debugging
        throw std::runtime_error("dot command failed with code " + std::to_string(result));
    }

    std::cout << "Done - cleaning up " << dotFilename << std::endl;
    std::remove(dotFilename.c_str());
}
} // namespace expr
