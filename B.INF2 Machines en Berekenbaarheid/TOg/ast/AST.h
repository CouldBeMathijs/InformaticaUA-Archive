// ast/AST.h
#ifndef AST_H
#define AST_H

#include "OperatorEnvironment.h"

#include <memory>

namespace expr {

enum class NodeType {
    Number,   // A numeric literal
    UnaryOp,  // A unary operator node
    BinaryOp, // A binary operator node
    Identifier,
    Group
};

/**
 * @brief Enum that represents all supported unary operators in the AST
 */
enum class UnaryOp {
    Plus, // Unary plus (+x)
    Minus // Unary minus (-x)
};

/**
 * @brief Enum that represents all supported binary operators in the AST
 */
enum class BinaryOp {
    Add,      // Addition (+)
    Subtract, // Subtraction (-)
    Multiply, // Multiplication (*)
    Divide    // Division (/)
};

/**
 * @bried Base class for all nodes in the AST
 */
class ASTNode {
  public:
    virtual ~ASTNode() = default;

    /**
     * @brief Returns the concrete type of this node
     */
    [[nodiscard]] virtual NodeType type() const = 0;

    /**
     * @brief String representation for debugging / GUI / tests
     */
    [[nodiscard]] virtual std::string toString() const = 0;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line tree layout
     *
     * @param indent Prefix spacing used for nested levels
     * @return A multi-line formatted string visualizing the AST
     */
    [[nodiscard]] virtual std::string prettyPrint(const std::string& indent) const = 0;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line layout
     * Simpler version of the function which takes indent as parameter
     * @return A multi-line formatted string visualizing the AST
     */
    [[nodiscard]] std::string prettyPrint() const { return prettyPrint(""); }

    /**
     * @brief Evaluates the node using a configurable operator environment
     *
     * This allows user defined operator behaviour and redefinition of built-in operators
     *
     * @param env The operator environment containing operator implementations
     * @return Computed numeric value of this AST subtree
     */
    [[nodiscard]] virtual double evaluateWith(const OperatorEnvironment& env) const = 0;

    /**
     * @brief Recursive helper to generate DOT graph nodes and edges.
     * @param out The stream to write DOT syntax to.
     * @param currentId A counter reference to ensure unique IDs for every node.
     * @return The unique ID of the node just written (to link parents to children).
     */
    virtual int toDot(std::ostream& out, int& currentId) const = 0;

    /**
     * @brief Exports the AST subtree rooted at this node to an image using Graphviz.
     * @param filename Output filename
     */
    void exportToImage(const std::string& filename) const;
};

/**
 * @brief AST node representing an explicit parenthesized / grouped sub-expression.
 */
class GroupNode final : public ASTNode {
  public:
    /**
     * @brief Constructs a GroupNode that wraps a child expression.
     *
     * @param child The expression inside the parentheses.
     */
    explicit GroupNode(std::unique_ptr<ASTNode> child);

    /**
     * @brief Returns the concrete type of this node.
     */
    [[nodiscard]] NodeType type() const override { return NodeType::Group; }

    /**
     * @brief String representation for debugging / GUI / tests.
     *
     * Transparent representation: does not introduce extra parentheses in the
     * canonical AST string form.
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line tree layout.
     *
     * Prints a "Group" node to make parentheses visible in debugging output.
     */
    [[nodiscard]] std::string prettyPrint(const std::string& indent) const override;

    /**
     * @brief Evaluates the grouped sub-expression (same value as its child).
     */
    [[nodiscard]] double evaluateWith(const OperatorEnvironment& env) const override;

    /**
     * @brief Recursive helper to generate DOT graph nodes and edges.
     */
    int toDot(std::ostream& out, int& currentId) const override;

  private:
    std::unique_ptr<ASTNode> child_;
};

/**
 * @brief AST node representing a numeric constant (e.g. 2, 3, 3.14, 10)
 */
class NumberNode final : public ASTNode {
  public:
    /**
     * @brief Constructs a NumberNode with a given numeric value
     * @param value The numerical constant stored in this node
     */
    explicit NumberNode(const double value)
        : value_(value) {}

    /**
     * @brief Returns the concrete type of this node
     */
    [[nodiscard]] NodeType type() const override { return NodeType::Number; }

    /**
     * @brief String representation for debugging / GUI / tests
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line tree layout
     *
     * @param indent Prefix spacing used for nested levels
     * @return A multi-line formatted string visualizing the AST
     */
    [[nodiscard]] std::string prettyPrint(const std::string& indent) const override;

    /**
     * @brief Evaluates this number using the given operator environment
     *
     * The environment has no effect on numeric literals.
     */
    [[nodiscard]] double evaluateWith(const OperatorEnvironment& env) const override;

    /**
     * @brief Recursive helper to generate DOT graph nodes and edges.
     * @param out The stream to write DOT syntax to.
     * @param currentId A counter reference to ensure unique IDs for every node.
     * @return The unique ID of the node just written (to link parents to children).
     */
    int toDot(std::ostream& out, int& currentId) const override;

  private:
    double value_;
};

/**
 * @brief Ast node representing a unary operation (e.g., -x or +x)
 */
class UnaryOpNode : public ASTNode {
  public:
    /**
     * @brief Constructs a UnaryOpNode with the given operator and child node
     *
     * @param op Unary operator (Plus or Minus)
     * @param child The Subtree on which the unary operator applies
     */
    UnaryOpNode(UnaryOp op, std::unique_ptr<ASTNode> child);

    /**
     * @brief Returns the concrete type of this node
     */
    [[nodiscard]] NodeType type() const override { return NodeType::UnaryOp; }

    /**
     * @brief String representation for debugging / GUI / tests
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line tree layout
     *
     * @param indent Prefix spacing used for nested levels
     * @return A multi-line formatted string visualizing the AST
     */
    [[nodiscard]] std::string prettyPrint(const std::string& indent) const override;

    /**
     * @brief Evaluates this unary operator using a given configurable environment
     *
     * If the operator is defined in the environment, that definition is used.
     * Otherwise, the built-in definitions are applied.
     */
    [[nodiscard]] double evaluateWith(const OperatorEnvironment& env) const override;

    /**
     * @brief Recursive helper to generate DOT graph nodes and edges.
     * @param out The stream to write DOT syntax to.
     * @param currentId A counter reference to ensure unique IDs for every node.
     * @return The unique ID of the node just written (to link parents to children).
     */
    int toDot(std::ostream& out, int& currentId) const override {
        int         myId   = currentId++;
        std::string symbol = (op_ == UnaryOp::Plus) ? "+" : "-";

        out << "  node" << myId << " [label=\"" << symbol << "\"];\n";

        if (child_) {
            int childId = child_->toDot(out, currentId);
            out << "  node" << myId << " -> node" << childId << ";\n";
        }
        return myId;
    }

  private:
    UnaryOp                  op_;
    std::unique_ptr<ASTNode> child_;
};

/**
 * @brief AST node representing a binary operation (e.g., left + right, left - right)
 */
class BinaryOpNode : public ASTNode {
  public:
    /**
     * @brief Constructs a BinaryOpNode
     *
     * @param op The binary operator (Add, Subtract, Multiply, Divide)
     * @param left Unique pointer to the left operand subtree
     * @param right Unique pointer to the right operand subtree
     */
    BinaryOpNode(BinaryOp op, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);

    /**
     * @brief Returns the concrete type of this node
     */
    [[nodiscard]] NodeType type() const override { return NodeType::BinaryOp; }

    /**
     * @brief String representation for debugging / GUI / tests
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line tree layout
     *
     * @param indent Prefix spacing used for nested levels
     * @return A multi-line formatted string visualizing the AST
     */
    [[nodiscard]] std::string prettyPrint(const std::string& indent) const override;

    /**
     * @brief Evaluates this binary operator using a configurable environment
     *
     * If the operator is defined in the environment, that definition is used
     * Otherwise, the build in definitions are applied
     */
    [[nodiscard]] double evaluateWith(const OperatorEnvironment& env) const override;

    /**
     * @brief Recursive helper to generate DOT graph nodes and edges.
     * @param out The stream to write DOT syntax to.
     * @param currentId A counter reference to ensure unique IDs for every node.
     * @return The unique ID of the node just written (to link parents to children).
     */
    int toDot(std::ostream& out, int& currentId) const override;

  private:
    BinaryOp                 op_;
    std::unique_ptr<ASTNode> left_;
    std::unique_ptr<ASTNode> right_;
};

/**
 * @brief AST node representing a custom unary operator identified by its symbol
 */
class CustomUnaryOpNode final : public ASTNode {
  public:
    /**
     * @brief Constructs a CustomUnaryOpNode
     *
     * @param symbol Symbol for the operator (e.g. "!", "~", "abs")
     * @param child Subtree on which the operator is applied
     */
    CustomUnaryOpNode(std::string symbol, std::unique_ptr<ASTNode> child);

    /**
     * @brief Returns the concrete type of this node
     */
    [[nodiscard]] NodeType type() const override { return NodeType::UnaryOp; }

    /**
     * @brief String representation for debugging / GUI / tests
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line tree layout.
     */
    [[nodiscard]] std::string prettyPrint(const std::string& indent) const override;

    /**
     * @brief Evaluates this node with a configurable operator environment.
     */
    [[nodiscard]] double evaluateWith(const OperatorEnvironment& env) const override;

    /**
     * @brief Recursive helper to generate DOT graph nodes and edges.
     * @param out The stream to write DOT syntax to.
     * @param currentId A counter reference to ensure unique IDs for every node.
     * @return The unique ID of the node just written (to link parents to children).
     */
    int toDot(std::ostream& out, int& currentId) const override;

  private:
    std::string              symbol_;
    std::unique_ptr<ASTNode> child_;
};

/**
 * @brief AST node representing a custom binary operator identified by its symbol
 */
class CustomBinaryOpNode : public ASTNode {
  public:
    /**
     * @brief Constructs a CustomBinaryOpNode
     *
     * @param symbol Symbol used for the operator (e.g. "%", "max")
     * @param left Subtree of the left operand
     * @param right Subtree of the right operand
     */
    CustomBinaryOpNode(std::string symbol, std::unique_ptr<ASTNode> left,
                       std::unique_ptr<ASTNode> right);

    /**
     * @brief Returns the concrete type of this node
     */
    [[nodiscard]] NodeType type() const override { return NodeType::BinaryOp; }

    /**
     * @brief String representation for debugging / GUI / tests
     */
    [[nodiscard]] std::string toString() const override;

    /**
     * @brief Pretty-prints this AST subtree in a multi-line tree layout.
     */
    [[nodiscard]] std::string prettyPrint(const std::string& indent) const override;

    /**
     * @brief String representation of this binary expression.
     */
    [[nodiscard]] double evaluateWith(const OperatorEnvironment& env) const override;

    /**
     * @brief Recursive helper to generate DOT graph nodes and edges.
     * @param out The stream to write DOT syntax to.
     * @param currentId A counter reference to ensure unique IDs for every node.
     * @return The unique ID of the node just written (to link parents to children).
     */
    int                              toDot(std::ostream& out, int& currentId) const override;

    [[nodiscard]] const std::string& symbol() const { return symbol_; }

    std::unique_ptr<ASTNode>         releaseRight() { return std::move(right_); }

    void                             setRight(std::unique_ptr<ASTNode> r) { right_ = std::move(r); }

  private:
    std::string              symbol_;
    std::unique_ptr<ASTNode> left_;
    std::unique_ptr<ASTNode> right_;
};

/**
 * @brief Creates a NumberNode wrapped in a smart pointer
 *
 * @param value Numeric value to store in the node
 * @return Unique pointer to a NumberNode
 */
inline std::unique_ptr<ASTNode> makeNumber(double value) {
    return std::make_unique<NumberNode>(value);
}

/**
 * @brief Creates a UnaryOpNode wrapped inside a unique_prt
 *
 * @param op Unary operator
 * @param child Child AST subtree
 * @return Unique pointer to a UnaryOpNode
 */
inline std::unique_ptr<ASTNode> makeUnary(UnaryOp op, std::unique_ptr<ASTNode> child) {
    return std::make_unique<UnaryOpNode>(op, std::move(child));
}

/**
 * @brief Creates a BinaryOpNode wrapped in a smart pointer
 *
 * @param op Operator to apply
 * @param left Left subtree
 * @param right Right subtree
 * @return Unique pointer to a BinaryOpNode
 */
inline std::unique_ptr<ASTNode> makeBinary(BinaryOp op, std::unique_ptr<ASTNode> left,
                                           std::unique_ptr<ASTNode> right) {
    return std::make_unique<BinaryOpNode>(op, std::move(left), std::move(right));
}

/**
 * @brief Creates a CustomUnaryOpNode wrapped in a smart pointer.
 *
 * @param symbol Symbol for the operator.
 * @param child Child subtree on which the operator applies.
 * @return Unique pointer to a CustomUnaryOpNode.
 */
inline std::unique_ptr<ASTNode> makeCustomUnary(std::string              symbol,
                                                std::unique_ptr<ASTNode> child) {
    return std::make_unique<CustomUnaryOpNode>(std::move(symbol), std::move(child));
}

/**
 * @brief Creates a CustomBinaryOpNode wrapped in a smart pointer.
 *
 * @param symbol Symbol for the operator.
 * @param left Left subtree.
 * @param right Right subtree.
 * @return Unique pointer to a CustomBinaryOpNode.
 */
inline std::unique_ptr<ASTNode> makeCustomBinary(std::string symbol, std::unique_ptr<ASTNode> left,
                                                 std::unique_ptr<ASTNode> right) {
    return std::make_unique<CustomBinaryOpNode>(std::move(symbol), std::move(left),
                                                std::move(right));
}
} // namespace expr

#endif
