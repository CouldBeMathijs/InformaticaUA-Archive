#ifndef OPERATOR_CONFIG_H
#define OPERATOR_CONFIG_H

#include <set>
#include <string>
#include <unordered_map>

namespace cfg {

/**
 * @brief Metadata for a binary operator.
 *
 * Used to store precedence and associativity information
 * for infix-style binary operators.
 */
struct BinOpMeta {
    /// Precedence weight (higher means higher priority)
    int weight = 0;

    /// True if operator is right-associative (e.g. exponentiation)
    bool rightAssoc = false;
};

/**
 * @brief Stores and manages operator definitions used by the expression system.
 *
 * OperatorConfig loads operator definitions from a JSON configuration file
 * (typically expression_grammar.json) and classifies operators into:
 * - binary (infix)
 * - unary (prefix)
 * - postfix (suffix)
 * - function-like operators with arity 1 or 2 (call notation)
 *
 * This configuration is shared by:
 * - CFG (token classification for CYK)
 * - SLRParser (lookahead classification: BIN, UN, POST, FUNC1, FUNC2)
 * - Shunting Yard (precedence / associativity)
 */
struct OperatorConfig {
    /**
     * @brief Construct an OperatorConfig by loading a JSON grammar file.
     *
     * @param filename Path to the JSON operator configuration file.
     *                 Defaults to "../config/expression_grammar.json".
     */
    explicit OperatorConfig(const std::string& filename = "../config/expression_grammar.json");

    /**
     * @brief Check if a lexeme is a binary (infix) operator.
     * @param lexeme Operator symbol (e.g. "+", "*")
     * @return True if binary operator
     */
    [[nodiscard]] bool isBinary(const std::string& lexeme) const;

    /**
     * @brief Check if a lexeme is a unary prefix operator.
     * @param lexeme Operator symbol (e.g. "-", "!")
     * @return True if unary prefix operator
     */
    [[nodiscard]] bool isUnary(const std::string& lexeme) const;

    /**
     * @brief Check if a lexeme is a postfix unary operator.
     * @param lexeme Operator symbol (e.g. "!")
     * @return True if postfix operator
     */
    [[nodiscard]] bool isPostfix(const std::string& lexeme) const;

    /**
     * @brief Check if a lexeme is a call-style operator with arity 1.
     *
     * Examples:
     *   sin(x), abs(x)
     *
     * @param lexeme Operator name or symbol
     * @return True if arity-1 call operator
     */
    [[nodiscard]] bool isFunc1(const std::string& lexeme) const;

    /**
     * @brief Check if a lexeme is a call-style operator with arity 2.
     *
     * Examples:
     *   min(a, b), max(a, b), $(a, b)
     *
     * @param lexeme Operator name or symbol
     * @return True if arity-2 call operator
     */
    [[nodiscard]] bool isFunc2(const std::string& lexeme) const;

    /**
     * @brief Get the precedence weight of a binary operator.
     *
     * @param lexeme Binary operator symbol
     * @return Precedence weight
     *
     * @warning Behaviour is undefined if lexeme is not a binary operator.
     */
    int binaryWeight(const std::string& lexeme) const;

    /**
     * @brief Check if a binary operator is right-associative.
     *
     * @param lexeme Binary operator symbol
     * @return True if right-associative
     *
     * @warning Behaviour is undefined if lexeme is not a binary operator.
     */
    bool binaryRightAssoc(const std::string& lexeme) const;

    /**
     * @brief Get a set containing all operator lexemes.
     *
     * This includes binary, unary, postfix and function-style operators.
     *
     * @return Set of all operator strings
     */
    [[nodiscard]] const std::set<std::string>& allOperatorLexemes() const;

    // ---------------------------
    // Operator classification sets
    // ---------------------------

    /// Infix-style binary operators (e.g. +, *, ^)
    std::set<std::string> bin;

    /// Prefix-style unary operators (e.g. -, !)
    std::set<std::string> un;

    /// Postfix-style unary operators (e.g. !)
    std::set<std::string> post;

    /// Call-style operators with arity 1 (e.g. sin(x))
    std::set<std::string> func1;

    /// Call-style operators with arity 2 (e.g. min(a,b), $(a,b))
    std::set<std::string> func2;

    /// Union of all operator lexemes
    std::set<std::string> all;

    /// Metadata for binary operators (precedence + associativity)
    std::unordered_map<std::string, BinOpMeta> binMeta;
};

} // namespace cfg

#endif
