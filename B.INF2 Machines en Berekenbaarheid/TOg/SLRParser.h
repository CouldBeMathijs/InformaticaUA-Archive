#ifndef MBTOG_SLRPARSER_H
#define MBTOG_SLRPARSER_H

#include "CFG.h"
#include "ast/AST.h"
#include "config/OperatorConfig.h"

#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

using namespace std;

/**
 * @brief Represents a single LR(0) item of the form:
 *        A → α • β
 */
struct LR0Item {
    /// Left-hand side non-terminal
    std::string head;

    /// Right-hand side symbols
    std::vector<std::string> body;

    /// Position of the dot in the production
    int dotPos;

    /**
     * @brief Construct an LR(0) item.
     * @param h Head (non-terminal)
     * @param b Body of the production
     * @param dot Position of the dot
     */
    LR0Item(std::string h, std::vector<std::string> b, int dot);

    /**
     * @brief Get the symbol immediately after the dot.
     * @return Symbol after the dot, or empty string if dot is at the end
     */
    [[nodiscard]] std::string getSymbolAfterDot() const;

    /**
     * @brief Advance the dot by one position.
     * @return New LR0Item with dot moved forward
     */
    [[nodiscard]] LR0Item getNextItem() const;

    /**
     * @brief Check if this item is a reduce item (dot at end).
     * @return True if dot is at the end of the body
     */
    [[nodiscard]] bool isReduceItem() const;

    /// Ordering operator for use in std::set
    bool operator<(const LR0Item& other) const;

    /// Equality operator
    bool operator==(const LR0Item& other) const;
};

/**
 * @brief Represents a state in the LR(0) automaton.
 */
struct LR0State {
    /// Unique state identifier
    int stateId;

    /// Set of LR(0) items in this state
    std::set<LR0Item> items;

    /**
     * @brief Compare two states for equality.
     * @param other Another LR0State
     * @return True if item sets are equal
     */
    bool operator==(const LR0State& other) const;
};

/**
 * @brief SLR(1) parser implementation.
 *
 * Builds FIRST/FOLLOW sets, LR(0) canonical collection,
 * ACTION/GOTO tables and parses token streams into an AST.
 */
class SLRParser {
  private:
    /// Context-free grammar used by the parser
    CFG cfg;

    /// FIRST sets for grammar symbols
    std::unordered_map<std::string, std::unordered_set<std::string>> firstSets;

    /// FOLLOW sets for non-terminals
    std::unordered_map<std::string, std::unordered_set<std::string>> followSets;

    /// Canonical collection of LR(0) states
    std::vector<LR0State> canonicalCollection;

    /// ACTION table: state × terminal → action
    std::map<int, std::map<std::string, std::string>> actionTable;

    /// GOTO table: state × non-terminal → next state
    std::map<int, std::map<std::string, int>> gotoTable;

    /// Operator configuration (prefix, postfix, call, etc.)
    const cfg::OperatorConfig* opConfig_ = nullptr;

    /// Enable verbose debug output
    bool debug = false;

  public:
    /**
     * @brief Construct an SLR parser from a CFG.
     * @param config Grammar configuration
     */
    explicit SLRParser(CFG config)
        : cfg(std::move(config)) {}

    /**
     * @brief Attach an operator configuration.
     * @param ops Pointer to OperatorConfig
     */
    void setOperatorConfig(const cfg::OperatorConfig* ops) { opConfig_ = ops; }

    /**
     * @brief Convert grammar productions into a vector form.
     * @return Vector of (head, body) pairs
     */
    std::vector<std::pair<std::string, std::vector<std::string>>> getProductionsVector() const;

    /**
     * @brief Compute FIRST set of a sequence of symbols.
     * @param symbols Grammar symbols
     * @return FIRST(symbols)
     */
    std::unordered_set<std::string> computeFirst(const std::vector<std::string>& symbols);

    /**
     * @brief Compute FIRST sets for all grammar symbols.
     */
    void computeFirstSets();

    /**
     * @brief Compute FOLLOW sets for all non-terminals.
     */
    void computeFollowSets();

    /**
     * @brief Compute closure of an LR(0) state.
     * @param state State to expand
     */
    void computeClosure(LR0State& state) const;

    /**
     * @brief Compute GOTO(state, symbol).
     * @param state Current LR(0) state
     * @param symbol Grammar symbol
     * @return Resulting LR(0) state
     */
    LR0State computeGoto(const LR0State& state, const std::string& symbol) const;

    /**
     * @brief Build the SLR parsing table.
     * @param io Output stream for diagnostics
     */
    void buildSLRTable(std::ostream& io = null_out);

    /**
     * @brief Print the ACTION and GOTO tables.
     * @param io Output stream
     */
    void printSLRTable(ostream& io) const;

    /**
     * @brief Get parsing action for a given state and symbol.
     * @param state Parser state
     * @param symbol Input symbol
     * @return Action string (shift, reduce, accept, error)
     */
    std::string getAction(int state, const std::string& symbol) const;

    /**
     * @brief Generate and print the SLR table.
     * @param io Output stream
     */
    void slr(std::ostream& io = null_out);

    /**
     * @brief Parse a token sequence into an AST.
     * @param inputTokens Tokenized input
     * @param io Output stream for debug/error messages
     * @return Root AST node, or nullptr on failure
     */
    std::unique_ptr<expr::ASTNode> parse(const std::vector<std::string>& inputTokens,
                                         std::ostream&                   io = std::cout) const;

    /**
     * @brief Validate parentheses balance in token stream.
     * @param tokens Input tokens
     * @param io Output stream for errors
     * @return True if parentheses are balanced
     */
    static bool validateParentheses(const std::vector<std::string>& tokens, std::ostream& io);

    /**
     * @brief Print input tokens with an error pointer.
     * @param tokens Input tokens
     * @param errorPos Position of the error
     * @param io Output stream
     */
    static void printInputWithPointer(const std::vector<std::string>& tokens, size_t errorPos,
                                      std::ostream& io);

    /**
     * @brief Check if a string represents a number.
     * @param s Input string
     * @return True if numeric
     */
    static bool isNumber(const std::string& s);

    /**
     * @brief Check if a string is a valid identifier.
     * @param s Input string
     * @return True if valid identifier
     */
    static bool isValidIdentifier(const std::string& s);

    /**
     * @brief Enable or disable debug output.
     * @param on True to enable debug mode
     */
    void setDebug(bool on) { debug = on; }
};

#endif // MBTOG_SLRPARSER_H
