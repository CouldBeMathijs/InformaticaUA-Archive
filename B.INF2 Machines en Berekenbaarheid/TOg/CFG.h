#ifndef CFG_H
#define CFG_H

#include "config/OperatorConfig.h"
#include "external/json.hpp"

#include <map>
#include <set>
#include <vector>

using json = nlohmann::json;

/**
 * @brief Null output stream used to suppress output when needed.
 */
inline std::ostream null_out(nullptr);

/**
 * @brief Context-Free Grammar (CFG) representation.
 *
 * This class encapsulates all data structures and algorithms required for:
 *  - Representing a context-free grammar
 *  - Loading grammar definitions from JSON
 *  - Converting grammars to Chomsky Normal Form (CNF)
 *  - Validating input strings using the CYK algorithm
 *  - Supporting operator precedence and associativity via configuration
 */
class CFG {
    std::set<std::string> variables; ///< Set of non-terminal symbols
    std::set<std::string> terminals; ///< Set of terminal symbols

    /**
     * @brief Set of grammar production rules.
     *
     * Each production is stored as a pair consisting of:
     *  - a head (left-hand side variable)
     *  - a body (sequence of terminals and/or variables)
     */
    std::set<std::pair<std::string, std::vector<std::string>>> productions;

    std::string startSymbol; ///< Start symbol of the grammar

  public:
    /**
     * @brief Default constructor.
     */
    CFG() = default;

    /**
     * @brief Constructs a CFG from a JSON file or JSON string.
     *
     * @param filename Path to the JSON file or JSON content
     * @param isFile Indicates whether the input is a file path
     */
    explicit CFG(const std::string& filename, bool isFile = true);

    /**
     * @brief Prints the grammar components to standard output.
     */
    void print() const;

    /**
     * @brief Converts the grammar to Chomsky Normal Form (CNF).
     */
    void toCNF();

    /**
     * @brief Sets the operator configuration used during parsing.
     *
     * @param ops Pointer to an OperatorConfig instance
     */
    void setOperatorConfig(const cfg::OperatorConfig* ops);

    /**
     * @brief Checks whether an input string is accepted using the CYK algorithm.
     *
     * @param input Input string
     * @return True if the string is accepted by the grammar
     */
    bool accepts(const std::string& input);

    /**
     * @brief Checks whether a tokenized input sequence is accepted using CYK.
     *
     * @param tokens Vector of input tokens
     * @return True if the token sequence is accepted
     */
    bool acceptsTokens(const std::vector<std::string>& tokens);

    /** @name Grammar Setters */
    ///@{
    void setVariables(const std::set<std::string>& newVariables);
    void setTerminals(const std::set<std::string>& newTerminals);
    void setStartSymbol(const std::string& newStartSymbol);
    ///@}

    /**
     * @brief Adds a production rule to the grammar.
     *
     * @param head Left-hand side variable
     * @param body Right-hand side symbols
     */
    void addProduction(const std::string& head, const std::vector<std::string>& body);

    /**
     * @brief Checks whether a production rule already exists.
     *
     * @param head Left-hand side variable
     * @param body Right-hand side symbols
     * @return True if the production exists
     */
    [[nodiscard]] bool hasProduction(const std::string&              head,
                                     const std::vector<std::string>& body) const;

    /**
     * @brief Replaces all productions with a new set.
     *
     * @param newProductions Set of production rules
     */
    void setProductions(
        const std::set<std::pair<std::string, std::vector<std::string>>>& newProductions);

    /**
     * @brief Returns the set of grammar variables.
     */
    [[nodiscard]] const std::set<std::string>& getVariables() const { return variables; }

    /**
     * @brief Returns the set of grammar terminals.
     */
    [[nodiscard]] const std::set<std::string>& getTerminals() const { return terminals; }

    /**
     * @brief Returns the grammar start symbol.
     */
    [[nodiscard]] const std::string& getStartSymbol() const { return startSymbol; }

    /**
     * @brief Returns all grammar productions.
     */
    [[nodiscard]] const std::set<std::pair<std::string, std::vector<std::string>>>& getProductions()
        const {
        return productions;
    }

    /**
     * @brief Loads a grammar definition from a JSON file.
     *
     * @param filename Path to the JSON file
     */
    void loadFromJsonFile(const std::string& filename);

    /**
     * @brief Loads a grammar definition from a JSON string.
     *
     * @param json_string JSON-formatted string
     */
    void loadFromJsonString(const std::string& json_string);

    /**
     * @brief Loads a grammar definition from a parsed JSON object.
     *
     * @param j Parsed JSON grammar
     */
    void loadFromJSON(const json& j);

    /**
     * @brief Prints a sorted set of strings.
     *
     * @param s Set to print
     */
    static void printSortedSet(const std::set<std::string>& s);

    /**
     * @brief Prints all grammar production rules.
     */
    void printProductions() const;

    /** @name CNF Transformation Methods */
    ///@{
    void eliminateEpsilonProductions();
    void eliminateUnitProductions();
    void eliminateUselessSymbols();
    void replaceTerminalsInBadBodies();
    void breakLongBodies();

    ///@}

    /**
     * @brief Returns the currently active operator configuration.
     */
    const cfg::OperatorConfig* getOperatorConfig() const { return opConfig_; }

    /** @name CNF Helper Methods */
    ///@{
    std::set<std::string>                                                findGeneratingSymbols();
    std::set<std::string>                                                findReachableSymbols();
    std::set<std::string>                                                findNullableSymbols();
    std::map<std::pair<std::string, std::string>, std::set<std::string>> findUnitPairs();
    std::string getNewVariable(const std::string& base);
    [[nodiscard]] std::vector<std::pair<std::string, std::vector<std::string>>>
    getProductionsByHead(const std::string& head) const;
    ///@}

    /** @name CYK Utilities */
    ///@{
    void displayCYKTable(const std::vector<std::vector<std::set<std::string>>>& table,
                         const std::string&                                     input);

    void displayCYKTable(const std::vector<std::vector<std::set<std::string>>>& table,
                         const std::vector<std::string>&                        tokens);

    [[nodiscard]] static std::vector<std::string> preprocessImplicitMul(
        const std::vector<std::string>& tokens);

    [[nodiscard]] static std::vector<std::string> preprocessImplicitMulForCYK_NUM_LPAREN(
        const std::vector<std::string>& tokens);
    ///@}

  private:
    const cfg::OperatorConfig* opConfig_ = nullptr; ///< Operator configuration pointer
};

#endif // CFG_H
