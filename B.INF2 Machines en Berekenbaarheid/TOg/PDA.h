#ifndef PDA_H
#define PDA_H

#include "CFG.h"

#include <set>
#include <string>
#include <vector>

/**
 * @brief Representation of a Pushdown Automaton (PDA) and related data structures.
 *
 * This header defines all core structures required for:
 *  - Formal PDA simulation
 *  - Step-by-step execution tracing
 *  - Visualization support
 *  - Conversion to a Context-Free Grammar (CFG)
 *  - Grammar-based validation using the CYK algorithm
 */

/**
 * @brief Structure representing a single transition in a Pushdown Automaton.
 *
 * Formally defined as:
 *      (p, a, A) → (q, γ)
 *
 * where:
 *  - p is the current state
 *  - a is the input symbol read (or ε)
 *  - A is the symbol on top of the stack
 *  - q is the next state
 *  - γ is the string that replaces A on the stack
 */
struct PDATransition {
    std::string              from;        ///< Current state (p)
    std::string              input;       ///< Input symbol (a) or ε
    std::string              stackTop;    ///< Stack top symbol (A)
    std::string              to;          ///< Next state (q)
    std::vector<std::string> replacement; ///< Stack replacement string (γ)
};

/**
 * @brief Structure representing a single step in a PDA simulation.
 *
 * This is primarily used for visualization and detailed execution tracing.
 */
struct SimulationStep {
    int                      stepNumber;     ///< Sequential step index
    std::string              currentState;   ///< State before applying the transition
    std::vector<std::string> stackContent;   ///< Current stack contents
    std::string              inputConsumed;  ///< Portion of the input already consumed
    std::string              inputRemaining; ///< Portion of the input yet to be processed
    std::string              action;         ///< Description of the applied transition
    std::string              nextState;      ///< Resulting state after the transition
    std::string              errorMessage;   ///< Error message if the step failed
};

/**
 * @brief Structure representing a single context-free grammar rule.
 *
 * A rule is of the form:
 *      head → body
 */
struct GrammarRule {
    std::string              head; ///< Left-hand side variable
    std::vector<std::string> body; ///< Right-hand side symbols
};

/**
 * @brief Pushdown Automaton (PDA) implementation.
 *
 * This class provides functionality for:
 *  - Loading PDA definitions from JSON
 *  - Simulating input strings step-by-step
 *  - Recording simulation traces
 *  - Visualizing PDA structure and execution
 *  - Converting the PDA to an equivalent CFG
 *  - Validating symbol sequences using CYK parsing
 */
class PDA {
  private:
    /**
     * @brief Formal PDA definition (7-tuple).
     */
    std::set<std::string>      states;          ///< Set of PDA states
    std::set<std::string>      alphabet;        ///< Input alphabet
    std::set<std::string>      stackAlphabet;   ///< Stack alphabet
    std::vector<PDATransition> transitions;     ///< Transition function
    std::string                startState;      ///< Initial state
    std::string                startStack;      ///< Initial stack symbol
    std::set<std::string>      acceptingStates; ///< Set of accepting states

    /**
     * @brief State related to the last simulation run.
     */
    std::vector<SimulationStep> lastSimulationSteps;    ///< Trace of last simulation
    mutable std::string         lastErrorMessage;       ///< Error message of last run
    bool                        lastSimulationAccepted; ///< Acceptance result

    /**
     * @brief Grammar data used for CYK-based validation.
     */
    std::vector<GrammarRule> grammarRules; ///< Grammar production rules
    std::set<std::string>    terminals;    ///< Terminal symbols
    std::set<std::string>    variables;    ///< Non-terminal symbols
    std::string              startSymbol;  ///< Grammar start symbol

    /**
     * @brief Validates structural correctness of a symbol sequence.
     *
     * @param symbols Tokenized input symbols
     * @return True if the structure is valid, false otherwise
     */
    bool validateStructure(const std::vector<std::string>& symbols) const;

    /**
     * @brief Inserts implicit multiplication symbols where required.
     *
     * @param tokens Vector of tokens or symbols
     * @param tokensAreSymbols Indicates whether tokens are already symbolic
     */
    static void insertImplicitMultiplication(std::vector<std::string>& tokens,
                                             bool                      tokensAreSymbols);

  public:
    /**
     * @brief Constructs a PDA from a JSON definition file.
     *
     * @param filename Path to the JSON file
     */
    explicit PDA(const std::string& filename);

    /**
     * @brief Default constructor.
     */
    PDA();

    /**
     * @brief Converts this PDA to an equivalent context-free grammar.
     *
     * @return CFG representation of the PDA
     */
    [[nodiscard]] CFG toCFG() const;

    /**
     * @brief Loads a PDA definition from a JSON file.
     *
     * @param filename Path to the JSON file
     */
    void loadFromJSON(const std::string& filename);

    /**
     * @brief Loads a context-free grammar from a JSON file.
     *
     * @param filename Path to the grammar JSON file
     */
    void loadGrammarFromJSON(const std::string& filename);

    /**
     * @brief Simulates the PDA on a given input string.
     *
     * @param input Input string to simulate
     * @return True if the input is accepted, false otherwise
     */
    bool simulate(const std::string& input);

    /**
     * @brief Checks whether a state is accepting.
     *
     * @param state State identifier
     * @return True if the state is accepting
     */
    bool isAcceptingState(const std::string& state) const;

    /**
     * @brief Returns the error message from the last simulation.
     *
     * @return Error message or empty string
     */
    [[nodiscard]] std::string getErrorMessage() const;

    /**
     * @brief Returns the full trace of the last simulation.
     *
     * @return Vector of simulation steps
     */
    [[nodiscard]] std::vector<SimulationStep> getLastSimulation() const;

    /**
     * @brief Adds accepting states to the PDA.
     *
     * @param states List of state identifiers
     */
    void addAcceptingStates(const std::vector<std::string>& states);

    /**
     * @brief Sets the accepting states of the PDA.
     *
     * @param states Set of state identifiers
     */
    void setAcceptingStates(const std::set<std::string>& states);

    /**
     * @brief Validates a symbol sequence using CYK parsing.
     *
     * @param cykSymbols Tokenized input symbols
     * @return True if the sequence is grammatically valid
     */
    bool validateGrammarStructure(const std::vector<std::string>& cykSymbols) const;

    /** @name Debugging Getters */
    ///@{
    [[nodiscard]] std::set<std::string>      getStates() const { return states; }

    [[nodiscard]] std::set<std::string>      getAlphabet() const { return alphabet; }

    [[nodiscard]] std::set<std::string>      getStackAlphabet() const { return stackAlphabet; }

    [[nodiscard]] std::vector<PDATransition> getTransitions() const { return transitions; }

    [[nodiscard]] std::string                getStartState() const { return startState; }

    [[nodiscard]] std::string                getStartStack() const { return startStack; }

    [[nodiscard]] bool wasLastSimulationAccepted() const { return lastSimulationAccepted; }

    ///@}

    /** @name Grammar Getters */
    ///@{
    [[nodiscard]] std::vector<GrammarRule> getGrammarRules() const { return grammarRules; }

    [[nodiscard]] std::set<std::string>    getTerminals() const { return terminals; }

    [[nodiscard]] std::set<std::string>    getVariables() const { return variables; }

    [[nodiscard]] std::string              getStartSymbol() const { return startSymbol; }

    ///@}

    /**
     * @brief Generates a Graphviz DOT representation of the PDA.
     *
     * @return DOT-format string
     */
    std::string toDotString() const;

    /**
     * @brief Exports the PDA structure to an image file.
     *
     * @param filename Output image filename
     */
    void exportToImage(const std::string& filename) const;

    /**
     * @brief Exports the last simulation trace as an image.
     *
     * @param filename Output image filename
     */
    void exportSimulationToImage(const std::string& filename) const;

    /**
     * @brief Simulates the PDA using CYK-compatible symbols.
     *
     * @param cykSymbols Tokenized CYK symbols
     * @return True if accepted
     */
    bool simulateCykSymbols(const std::vector<std::string>& cykSymbols);
};

#endif // PDA_H
