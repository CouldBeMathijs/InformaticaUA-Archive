#include "PDA.h"

#include "external/json.hpp"

#include <algorithm>
#include <cctype>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_set>

#ifdef _WIN32
constexpr std::string_view DOT_PATH = "dot";
#else
constexpr std::string_view DOT_PATH = "dot";
#endif

using json = nlohmann::json;

// CONSTRUCTORS
PDA::PDA(const std::string& filename)
    : lastSimulationAccepted(false) {
    loadFromJSON(filename);
}

PDA::PDA()
    : lastSimulationAccepted(false) {
    // Load grammar from expression_grammar.json
    loadGrammarFromJSON("config/expression_grammar.json");
}

// CORE PDA METHODS
void PDA::loadFromJSON(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    json j;
    input >> j;

    states.clear();
    alphabet.clear();
    stackAlphabet.clear();
    transitions.clear();
    acceptingStates.clear();

    if (j.contains("States") && j["States"].is_array())
        for (const auto& state : j["States"])
            states.insert(state.get<std::string>());

    if (j.contains("Alphabet") && j["Alphabet"].is_array())
        for (const auto& symbol : j["Alphabet"])
            alphabet.insert(symbol.get<std::string>());

    if (j.contains("StackAlphabet") && j["StackAlphabet"].is_array())
        for (const auto& symbol : j["StackAlphabet"])
            stackAlphabet.insert(symbol.get<std::string>());

    if (j.contains("Transitions") && j["Transitions"].is_array()) {
        for (const auto& trans : j["Transitions"]) {
            PDATransition transition;

            if (trans.contains("from"))
                transition.from = trans["from"].get<std::string>();
            if (trans.contains("input"))
                transition.input = trans["input"].get<std::string>();
            if (trans.contains("stacktop"))
                transition.stackTop = trans["stacktop"].get<std::string>();
            if (trans.contains("to"))
                transition.to = trans["to"].get<std::string>();

            if (trans.contains("replacement") && trans["replacement"].is_array())
                for (const auto& symbol : trans["replacement"])
                    transition.replacement.push_back(symbol.get<std::string>());

            transitions.push_back(transition);
        }
    }

    if (j.contains("StartState"))
        startState = j["StartState"].get<std::string>();

    if (j.contains("StartStack"))
        startStack = j["StartStack"].get<std::string>();

    if (j.contains("AcceptingStates") && j["AcceptingStates"].is_array())
        for (const auto& state : j["AcceptingStates"])
            acceptingStates.insert(state.get<std::string>());
}

// LOAD GRAMMAR FROM expression_grammar.json
void PDA::loadGrammarFromJSON(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open())
        throw std::runtime_error("Cannot open grammar file: " + filename);

    json j;
    input >> j;

    // Clear existing grammar
    grammarRules.clear();
    terminals.clear();
    variables.clear();
    startSymbol = "";

    // Load Terminals
    if (j.contains("Terminals") && j["Terminals"].is_array())
        for (const auto& term : j["Terminals"])
            terminals.insert(term.get<std::string>());

    // Load Variables
    if (j.contains("Variables") && j["Variables"].is_array())
        for (const auto& var : j["Variables"])
            variables.insert(var.get<std::string>());

    // Load Start Symbol
    if (j.contains("Start"))
        startSymbol = j["Start"].get<std::string>();

    // Load Productions
    if (j.contains("Productions") && j["Productions"].is_array()) {
        for (const auto& prod : j["Productions"]) {
            std::string              head = prod["head"].get<std::string>();
            std::vector<std::string> body;

            if (prod.contains("body") && prod["body"].is_array())
                for (const auto& sym : prod["body"])
                    body.push_back(sym.get<std::string>());

            grammarRules.push_back({head, body});
        }
    }
}

// Accepting state management & result access
bool PDA::isAcceptingState(const std::string& state) const {
    return acceptingStates.contains(state);
}

void PDA::addAcceptingStates(const std::vector<std::string>& stateList) {
    for (const auto& state : stateList)
        acceptingStates.insert(state);
}

void PDA::setAcceptingStates(const std::set<std::string>& stateSet) { acceptingStates = stateSet; }

std::string                 PDA::getErrorMessage() const { return lastErrorMessage; }

std::vector<SimulationStep> PDA::getLastSimulation() const { return lastSimulationSteps; }

// PURE CYK SYMBOL VALIDATION
bool PDA::validateGrammarStructure(const std::vector<std::string>& cykSymbols) const {
    if (cykSymbols.empty()) {
        lastErrorMessage = "CYK symbool lijst is leeg";
        return false;
    }

    for (const auto& symbol : cykSymbols) {
        if (!terminals.contains(symbol)) {
            lastErrorMessage = "Ongeldig symbol: '" + symbol + "' (niet in grammar terminals)";
            return false;
        }
    }

    // Controleer structuur
    return validateStructure(cykSymbols);
}

bool PDA::validateStructure(const std::vector<std::string>& symbols) const {
    int         parenBalance     = 0;
    bool        expectingOperand = true;
    std::string currentFunc; // "FUNC1" or "FUNC2"
    int         paramCount   = 0;
    bool        inFuncParams = false;

    for (size_t i = 0; i < symbols.size(); ++i) {
        const auto& sym = symbols[i];

        // PARENTHESES
        if (sym == "(") {
            parenBalance++;

            if (!currentFunc.empty()) {
                inFuncParams     = true;
                paramCount       = 0;
                expectingOperand = true;
            } else {
                expectingOperand = true;
            }
        } else if (sym == ")") {
            parenBalance--;
            if (parenBalance < 0) {
                lastErrorMessage = "Te veel sluitende haakjes";
                return false;
            }

            // End of function parameters?
            if (inFuncParams && !currentFunc.empty()) {
                // Validate parameter count based on grammar
                if (currentFunc == "FUNC1" && paramCount != 1) {
                    lastErrorMessage = "FUNC1 verwacht 1 parameter, kreeg " +
                                       std::to_string(paramCount);
                    return false;
                }
                if (currentFunc == "FUNC2" && paramCount != 2) {
                    lastErrorMessage = "FUNC2 verwacht 2 parameters, kreeg " +
                                       std::to_string(paramCount);
                    return false;
                }

                // Reset for next function
                currentFunc.clear();
                inFuncParams = false;
                paramCount   = 0;
            }

            expectingOperand = false;
        }

        // COMMA
        else if (sym == ",") {
            if (!inFuncParams) {
                lastErrorMessage = "Komma buiten functie parameters";
                return false;
            }
            if (expectingOperand) {
                lastErrorMessage = "Komma op onverwachte positie";
                return false;
            }
            expectingOperand = true;
        }

        // OPERAND (NUM or ID)
        else if (sym == "NUM" || sym == "ID") {
            if (!expectingOperand) {
                lastErrorMessage = sym + " op onverwachte positie";
                return false;
            }

            if (inFuncParams)
                paramCount++;

            expectingOperand = false;
        }

        // FUNCTION
        else if (sym == "FUNC1" || sym == "FUNC2") {
            if (!expectingOperand) {
                lastErrorMessage = "Functie op onverwachte positie";
                return false;
            }

            // Function moet gevolgd worden door (
            if (i + 1 >= symbols.size() || symbols[i + 1] != "(") {
                lastErrorMessage = sym + " moet gevolgd worden door '('";
                return false;
            }

            currentFunc      = sym;
            expectingOperand = false;
        }

        // BINARY OPERATOR
        else if (sym == "OP_BIN") {
            if (expectingOperand) {
                lastErrorMessage = "OP_BIN op onverwachte positie (operand verwacht)";
                return false;
            }

            // Check: vorige symbol mag geen operator zijn
            if (i > 0 && (symbols[i - 1] == "OP_BIN" || symbols[i - 1] == "OP_UN_PRE" ||
                          symbols[i - 1] == "OP_UN_POST" || symbols[i - 1] == "OP_UN")) {
                lastErrorMessage = "Twee operators na elkaar";
                return false;
            }

            expectingOperand = true;
        }

        // PREFIX UNARY OPERATOR (bijv. -5, +3, !x)
        else if (sym == "OP_UN_PRE") {
            if (!expectingOperand) {
                lastErrorMessage = "OP_UN_PRE op onverwachte positie (operand verwacht)";
                return false;
            }

            // Prefix operator moet operand volgen
            if (i + 1 >= symbols.size()) {
                lastErrorMessage = "OP_UN_PRE zonder operand";
                return false;
            }

            // Expecting operand blijft true - na OP_UN_PRE komt de operand
            expectingOperand = true;
        }

        // POSTFIX UNARY OPERATOR (bijv. x%, y!, etc.)
        else if (sym == "OP_UN_POST") {
            if (expectingOperand) {
                lastErrorMessage = "OP_UN_POST op onverwachte positie (moet na operand komen)";
                return false;
            }

            // Postfix operator volgt op operand, dus verwachten we nu operator of einde
            expectingOperand = false;
        }

        // LEGACY UNARY OPERATOR (oude naam - voor compatibiliteit)
        else if (sym == "OP_UN") {
            // Behandel dit als prefix operator voor compatibiliteit
            if (!expectingOperand) {
                lastErrorMessage = "OP_UN op onverwachte positie (operand verwacht)";
                return false;
            }

            if (i + 1 >= symbols.size()) {
                lastErrorMessage = "OP_UN zonder operand";
                return false;
            }

            expectingOperand = true;
        }

        else {
            lastErrorMessage = "Onbekend symbol: '" + sym + "'";
            return false;
        }
    }

    // FINAL CHECKS
    if (parenBalance != 0) {
        lastErrorMessage = "Haakjes niet in balans";
        return false;
    }

    if (expectingOperand) {
        lastErrorMessage = "Expressie eindigt terwijl operand verwacht";
        return false;
    }

    if (!currentFunc.empty()) {
        lastErrorMessage = "Functie niet correct afgesloten";
        return false;
    }

    return true;
}

void PDA::insertImplicitMultiplication(std::vector<std::string>& tokens, bool tokensAreSymbols) {
    auto isNumberLiteral = [](std::string_view s) {
        if (s.empty())
            return false;
        // keep it consistent with your code: digits only
        return std::ranges::all_of(s, [](unsigned char c) { return std::isdigit(c); });
    };

    auto isNum        = [&](const std::string& t) { return t == "NUM" || isNumberLiteral(t); };

    auto isOperandEnd = [&](const std::string& a) {
        // raw: number or ')'
        // symbols: NUM, ')', OP_UN_POST
        if (a == ")")
            return true;
        if (isNum(a))
            return true;
        if (tokensAreSymbols && a == "OP_UN_POST")
            return true;
        return false;
    };

    auto isOperandStart = [&](const std::string& b, const std::string& a) {
        // raw: '(' or function or (sometimes) unary pre
        // symbols: '(', OP_UN_PRE, FUNC1/FUNC2
        if (b == "(")
            return true;

        // allow function calls as atom starters
        if (tokensAreSymbols && (b == "FUNC1" || b == "FUNC2"))
            return true;

        // allow unary prefix (only in symbol mode)
        if (tokensAreSymbols && b == "OP_UN_PRE")
            return true;

        // allow numbers as atom starters
        if (isNum(b))
            return !isNum(a);

        return false;
    };

    const std::string        opToInsert = tokensAreSymbols ? "OP_BIN" : "*";

    std::vector<std::string> out;
    out.reserve(tokens.size() * 2);

    for (size_t i = 0; i < tokens.size(); ++i) {
        out.push_back(tokens[i]);

        if (i + 1 >= tokens.size())
            continue;

        const std::string& a = tokens[i];
        const std::string& b = tokens[i + 1];

        if (isOperandEnd(a) && isOperandStart(b, a))
            out.push_back(opToInsert);
    }

    tokens = std::move(out);
}

// SIMULATION
bool PDA::simulate(const std::string& input) {
    // Reset last run state
    lastSimulationSteps.clear();
    lastErrorMessage       = "";
    lastSimulationAccepted = false;

    // helpers
    auto splitWhitespace = [](const std::string& s) {
        std::istringstream       iss(s);
        std::vector<std::string> out;
        for (std::string tok; iss >> tok;)
            out.push_back(tok);
        return out;
    };

    auto isNumberLiteral = [](std::string_view s) {
        if (s.empty())
            return false;

        return std::ranges::all_of(s, [](const unsigned char c) { return std::isdigit(c); });
    };

    auto isIdentLiteral = [](const std::string& s) {
        if (s.empty())
            return false;
        if (const auto c0 = static_cast<unsigned char>(s[0]); !(std::isalpha(c0) || s[0] == '_'))
            return false;
        for (size_t i = 1; i < s.size(); ++i)
            if (const auto c = static_cast<unsigned char>(s[i]); !(std::isalnum(c) || s[i] == '_'))
                return false;
        return true;
    };

    // Lexer for raw expressions without spaces: "123+45*(6)"
    auto lexRaw = [](const std::string& s) {
        std::vector<std::string> out;
        size_t                   i = 0;
        while (i < s.size()) {
            const auto c = static_cast<unsigned char>(s[i]);
            if (std::isspace(c)) {
                ++i;
                continue;
            }

            if (std::isdigit(c)) {
                size_t j = i + 1;
                while (j < s.size() && std::isdigit(static_cast<unsigned char>(s[j])))
                    ++j;
                out.push_back(s.substr(i, j - i));
                i = j;
                continue;
            }

            if (std::isalpha(c) || s[i] == '_') {
                size_t j = i + 1;
                while (j < s.size()) {
                    auto cj = static_cast<unsigned char>(s[j]);
                    if (!(std::isalnum(cj) || s[j] == '_'))
                        break;
                    ++j;
                }
                out.push_back(s.substr(i, j - i));
                i = j;
                continue;
            }

            // single-char symbol
            out.emplace_back(1, s[i]);
            ++i;
        }
        return out;
    };

    auto allAreAlphabetSymbols = [&](const std::vector<std::string>& tokens) {
        if (tokens.empty())
            return false;

        return std::ranges::all_of(tokens, [&](const auto& t) { return alphabet.contains(t); });
    };

    auto joinTokens = [](const std::vector<std::string>& tokens) {
        std::string s;
        for (const auto& t : tokens)
            s += t;
        return s;
    };

    // Normalize raw tokens -> PDA symbols (NUM / OP_BIN / OP_UN_PRE / OP_UN_POST / parentheses)
    auto normalizeToSymbols =
        [&](const std::vector<std::string>& tokens) -> std::vector<std::string> {
        std::vector<std::string> sym;
        sym.reserve(tokens.size());

        bool expectingOperand = true;

        auto isPostfixOpChar  = [](const std::string& t) { return (t == "!" || t == "%"); };

        auto isBinaryOpChar   = [](const std::string& t) {
            return (t == "+" || t == "-" || t == "*" || t == "/" || t == "^");
        };

        for (const auto& t : tokens) {
            if (t == "(" || t == ")" || t == ",") {
                sym.push_back(t);
                if (t == "(")
                    expectingOperand = true;
                if (t == ")")
                    expectingOperand = false;
                if (t == ",")
                    expectingOperand = true;
                continue;
            }

            if (isNumberLiteral(t)) {
                sym.emplace_back("NUM");
                expectingOperand = false;
                continue;
            }

            if (isIdentLiteral(t)) {
                lastErrorMessage = "Identifiers are not supported";
                return {};
            }

            // operators
            if (t == "+" || t == "-") {
                // unary if we are expecting an operand
                sym.emplace_back(expectingOperand ? "OP_UN_PRE" : "OP_BIN");
                expectingOperand = true;
                continue;
            }

            if (isPostfixOpChar(t)) {
                sym.emplace_back("OP_UN_POST");
                expectingOperand = false;
                continue;
            }

            if (isBinaryOpChar(t)) {
                sym.emplace_back("OP_BIN");
                expectingOperand = true;
                continue;
            }

            sym.push_back(t);
        }

        return sym;
    };

    // build token stream
    std::vector<std::string> rawTokens =
        (input.find(' ') != std::string::npos) ? splitWhitespace(input) : lexRaw(input);

    const bool alreadySymbols = allAreAlphabetSymbols(rawTokens);

    // insert implicit multiplication
    insertImplicitMultiplication(rawTokens, alreadySymbols);

    std::vector<std::string> tokens;
    if (alreadySymbols) {
        tokens = rawTokens; // contains OP_BIN inserted if needed
    } else {
        tokens = normalizeToSymbols(rawTokens); // "*" will become OP_BIN here
        if (tokens.empty() && !lastErrorMessage.empty()) {
            lastSimulationAccepted = false;
            return false;
        }
    }

    // simulation core
    struct ComputationState {
        std::string              state;
        size_t                   tokenIndex = 0;
        std::vector<std::string> stack;
    };

    std::vector<ComputationState> activeStates;
    activeStates.push_back({startState, 0, {startStack}});

    constexpr int MAX_STEPS       = 50000;
    int           globalStepCount = 0;
    int           stepNum         = 1;

    auto          findTransitions = [&](const std::string& state, const std::string& in,
                               const std::string& topStack) -> std::vector<const PDATransition*> {
        std::vector<const PDATransition*> result;
        for (const auto& trans : transitions)
            if (trans.from == state && trans.stackTop == topStack && trans.input == in)
                result.push_back(&trans);
        return result;
    };

    // Initial step
    {
        SimulationStep step;
        step.stepNumber     = stepNum;
        step.currentState   = startState;
        step.stackContent   = {startStack};
        step.inputConsumed  = "";
        step.inputRemaining = joinTokens(tokens);
        step.action         = "init";
        step.nextState      = startState;
        step.errorMessage   = "";
        lastSimulationSteps.push_back(step);
    }

    while (!activeStates.empty() && globalStepCount < MAX_STEPS) {
        globalStepCount++;
        stepNum++;

        std::vector<ComputationState> nextActiveStates;

        for (const auto& comp : activeStates) {
            const bool allInputConsumed = (comp.tokenIndex >= tokens.size());
            const bool stackEmpty       = comp.stack.empty();

            // ACCEPT: all input consumed + empty stack
            if (allInputConsumed && stackEmpty) {
                SimulationStep acceptStep;
                acceptStep.stepNumber     = stepNum;
                acceptStep.currentState   = comp.state;
                acceptStep.stackContent   = {};
                acceptStep.inputConsumed  = joinTokens(tokens);
                acceptStep.inputRemaining = "";
                acceptStep.action         = "accept"; // EXACT for tests
                acceptStep.nextState      = comp.state;
                acceptStep.errorMessage   = "";
                lastSimulationSteps.push_back(acceptStep);

                lastErrorMessage       = "";
                lastSimulationAccepted = true;
                return true;
            }

            // EPSILON transitions
            if (!stackEmpty) {
                std::string topStack = comp.stack.back();
                auto        epsTrans = findTransitions(comp.state, "", topStack);

                for (const auto* trans : epsTrans) {
                    ComputationState next = comp;
                    next.state            = trans->to;

                    // pop
                    next.stack.pop_back();

                    // push replacement IN GIVEN ORDER (IMPORTANT FIX)
                    for (const auto& symb : trans->replacement)
                        next.stack.push_back(symb);

                    nextActiveStates.push_back(std::move(next));
                }
            }

            // INPUT transitions
            if (!allInputConsumed && !stackEmpty) {
                std::string        topStack     = comp.stack.back();
                const std::string& currentToken = tokens[comp.tokenIndex];

                for (auto        inTrans = findTransitions(comp.state, currentToken, topStack);
                     const auto* trans : inTrans) {
                    ComputationState next = comp;
                    next.state            = trans->to;
                    next.tokenIndex       = comp.tokenIndex + 1;

                    // pop
                    next.stack.pop_back();

                    // push replacement
                    for (const auto& symb : trans->replacement)
                        next.stack.push_back(symb);

                    nextActiveStates.push_back(std::move(next));
                }
            }
        }

        // Remove duplicate configurations
        std::ranges::sort(nextActiveStates,
                          [](const ComputationState& a, const ComputationState& b) {
                              if (a.state != b.state)
                                  return a.state < b.state;
                              if (a.tokenIndex != b.tokenIndex)
                                  return a.tokenIndex < b.tokenIndex;
                              return a.stack < b.stack;
                          });

        nextActiveStates.erase(
            std::ranges::unique(nextActiveStates,
                                [](const ComputationState& a, const ComputationState& b) {
                                    return a.state == b.state && a.tokenIndex == b.tokenIndex &&
                                           a.stack == b.stack;
                                })
                .begin(),
            nextActiveStates.end());

        // optional cap to prevent explosion
        if (nextActiveStates.size() > 100)
            nextActiveStates.resize(100);

        activeStates = std::move(nextActiveStates);

        // Log one representative step (first active state)
        if (!activeStates.empty()) {
            SimulationStep step;
            step.stepNumber   = stepNum;
            step.currentState = activeStates[0].state;
            step.stackContent = activeStates[0].stack;

            std::string consumed;
            for (size_t j = 0; j < activeStates[0].tokenIndex && j < tokens.size(); ++j)
                consumed += tokens[j];
            step.inputConsumed = consumed;

            std::string remaining;
            for (size_t j = activeStates[0].tokenIndex; j < tokens.size(); ++j)
                remaining += tokens[j];
            step.inputRemaining = remaining;

            step.action         = "explore";
            step.nextState      = activeStates[0].state;
            step.errorMessage   = "";
            lastSimulationSteps.push_back(step);
        }
    }

    // REJECT
    if (globalStepCount >= MAX_STEPS) {
        lastErrorMessage = "PDA simulation exceeded maximum steps (" + std::to_string(MAX_STEPS) +
                           ")";
    } else if (lastErrorMessage.empty()) {
        lastErrorMessage = "PDA rejected: no accepting computation path";
    }

    SimulationStep rejectStep;
    rejectStep.stepNumber     = stepNum;
    rejectStep.currentState   = activeStates.empty() ? "" : activeStates[0].state;
    rejectStep.stackContent   = activeStates.empty() ? std::vector<std::string>{}
                                                     : activeStates[0].stack;
    rejectStep.inputConsumed  = joinTokens(tokens);
    rejectStep.inputRemaining = "";
    rejectStep.action         = "reject";
    rejectStep.nextState      = "";
    rejectStep.errorMessage   = lastErrorMessage;
    lastSimulationSteps.push_back(rejectStep);

    lastSimulationAccepted = false;
    return false;
}

// CFG CONVERSION

CFG PDA::toCFG() const {
    CFG                   cfg;

    std::set<std::string> cfgVars;
    std::set<std::string> terminals_set = alphabet;

    cfgVars.insert("S");
    cfg.setStartSymbol("S");

    for (const auto& p : states) {
        for (const auto& A : stackAlphabet) {
            for (const auto& q : states) {
                std::stringstream varName;
                varName << "[" << p << "," << A << "," << q << "]";
                cfgVars.insert(varName.str());
            }
        }
    }

    std::set<std::pair<std::string, std::vector<std::string>>> newProductions;

    for (const auto& q : states) {
        std::stringstream varName;
        varName << "[" << startState << "," << startStack << "," << q << "]";
        newProductions.insert({"S", {varName.str()}});
    }

    for (const auto& transition : transitions) {
        if (transition.replacement.empty()) {
            for (const auto& q : states) {
                if (transition.to == q) {
                    std::stringstream varName;
                    varName << "[" << transition.from << "," << transition.stackTop << "," << q
                            << "]";
                    newProductions.insert({varName.str(), {transition.input}});
                }
            }
        }
    }

    for (const auto& transition : transitions) {
        if (!transition.replacement.empty()) {
            if (transition.replacement.size() == 1) {
                for (const auto& q : states) {
                    std::stringstream leftVar;
                    leftVar << "[" << transition.from << "," << transition.stackTop << "," << q
                            << "]";

                    std::stringstream rightVar;
                    rightVar << "[" << transition.to << "," << transition.replacement[0] << "," << q
                             << "]";

                    newProductions.insert({leftVar.str(), {transition.input, rightVar.str()}});
                }
            } else if (transition.replacement.size() == 2) {
                for (const auto& q : states) {
                    for (const auto& s : states) {
                        std::stringstream leftVar;
                        leftVar << "[" << transition.from << "," << transition.stackTop << "," << q
                                << "]";

                        std::stringstream rightVar1;
                        rightVar1 << "[" << transition.to << "," << transition.replacement[0] << ","
                                  << s << "]";

                        std::stringstream rightVar2;
                        rightVar2 << "[" << s << "," << transition.replacement[1] << "," << q
                                  << "]";

                        newProductions.insert(
                            {leftVar.str(), {transition.input, rightVar1.str(), rightVar2.str()}});
                    }
                }
            }
        }
    }

    cfg.setVariables(cfgVars);
    cfg.setTerminals(terminals_set);
    cfg.setProductions(newProductions);

    return cfg;
}

std::string PDA::toDotString() const {
    std::ostringstream dot;

    dot << "digraph PDA {\n";
    dot << "  bgcolor=\"#0c0d10\";\n";
    dot << "  fontcolor=white;\n";
    dot << "  rankdir=LR;\n"; // Links naar rechts layout
    dot << "  ranksep=0.8;\n";
    dot << "  nodesep=0.5;\n\n";

    // Default node en edge style
    dot << "  node [shape=circle, style=filled, fillcolor=\"#141414\", "
        << "color=lightgray, fontcolor=white, penwidth=1.5, fontsize=10];\n";
    dot << "  edge [color=lightgray, fontcolor=white, penwidth=1.2, fontsize=9];\n\n";

    // Start node (dummy node die naar startState wijst)
    dot << "  start [label=\"START\", shape=point, fillcolor=white];\n";
    dot << "  start -> \"" << startState << "\";\n\n";

    // States
    dot << "  // States\n";
    for (const auto& state : states) {
        dot << "  \"" << state << "\" [";

        // Accepting states in groen
        if (acceptingStates.contains(state)) {
            dot << R"(fillcolor="#4CAF50", color="#2E7D32")";
        } else if (state == startState) {
            // Start state in blauw
            dot << R"(fillcolor="#2196F3", color="#1565C0")";
        }

        dot << "];\n";
    }

    dot << "\n  // Transitions\n";
    for (const auto& trans : transitions) {
        // Label: input/stack_top → replacement
        std::ostringstream label;
        label << trans.input << " | " << trans.stackTop << " → ";

        if (trans.replacement.empty()) {
            label << "ε";
        } else {
            for (size_t i = 0; i < trans.replacement.size(); ++i) {
                if (i > 0)
                    label << "";
                label << trans.replacement[i];
            }
        }

        dot << "  \"" << trans.from << "\" -> \"" << trans.to << "\" "
            << "[label=\"" << label.str() << "\"];\n";
    }

    dot << "}\n";
    return dot.str();
}

void PDA::exportToImage(const std::string& filename) const {
    std::filesystem::path outPath(filename);
    std::string           extension = outPath.extension().string();

    // Remove the extension point
    if (!extension.empty() && extension[0] == '.')
        extension.erase(0, 1);

    static const std::unordered_set<std::string> supported = {"png",  "jpg", "jpeg",
                                                              "webp", "svg", "pdf"};

    if (!supported.contains(extension))
        throw std::invalid_argument("Unsupported extension: " + extension);

    std::string dotContent  = toDotString();
    std::string baseName    = outPath.stem().string();
    std::string dotFilename = "temp_pda_" + baseName + ".dot";

    // Write DOT-bestand
    {
        std::ofstream ofs(dotFilename);
        if (!ofs)
            throw std::runtime_error("Failed to write temporary dot file: " + dotFilename);
        ofs << dotContent;
    }

    std::cout << "[PDA] " << dotFilename << " generated\n";

    // Add DPI for raster images
    std::string settings;
    if (extension == "png" || extension == "jpg" || extension == "webp")
        settings = " -Gdpi=300 ";

    // Run dot
    std::string command = std::string(DOT_PATH) + " -T" + extension + settings + " \"" +
                          dotFilename + "\" -o \"" + filename + "\"";

    std::cout << "[PDA] Executing: " << command << "\n";

    if (int result = std::system(command.c_str()); result != 0) {
        std::cerr << "[PDA] DOT command failed. Check that Graphviz is installed.\n";
        throw std::runtime_error("dot command failed with code " + std::to_string(result));
    }

    std::cout << "[PDA] Successfully created " << filename << "\n";
    std::remove(dotFilename.c_str());
}

void PDA::exportSimulationToImage(const std::string& filename) const {
    if (lastSimulationSteps.empty())
        throw std::runtime_error("No simulation data available. Run simulate() first.");

    std::ostringstream dot;
    dot << "digraph PDASimulation {\n";
    dot << "  bgcolor=\"#0c0d10\";\n";
    dot << "  fontcolor=white;\n";
    dot << "  rankdir=TB;\n";
    dot << "  ranksep=0.6;\n";
    dot << "  nodesep=0.3;\n\n";

    dot << "  node [shape=box, style=filled, fillcolor=\"#1a1a1a\", "
        << "color=lightgray, fontcolor=white, penwidth=1.2, fontsize=9];\n";
    dot << "  edge [color=lightgray, fontcolor=white, penwidth=1.2, fontsize=8];\n\n";

    // Create nodes for each step
    for (size_t i = 0; i < lastSimulationSteps.size(); ++i) {
        const auto&        step = lastSimulationSteps[i];

        std::ostringstream nodeLabel;
        nodeLabel << "Step " << step.stepNumber << "\\n";
        nodeLabel << "State: " << step.currentState << "\\n";
        nodeLabel << "Input: [" << step.inputRemaining << "]\\n";
        nodeLabel << "Stack: [";

        for (size_t j = 0; j < step.stackContent.size(); ++j) {
            if (j > 0)
                nodeLabel << "|";
            nodeLabel << step.stackContent[j];
        }
        nodeLabel << "]\\n";
        nodeLabel << "Action: " << step.action;

        std::string fillColor = "#141414";
        if (step.action == "accept")
            fillColor = "#4CAF50";
        else if (step.action == "reject")
            fillColor = "#F44336";

        dot << "  step" << i << " [label=\"" << nodeLabel.str() << "\", fillcolor=\"" << fillColor
            << "\"];\n";

        // Make edge to next step
        if (i + 1 < lastSimulationSteps.size())
            dot << "  step" << i << " -> step" << (i + 1) << ";\n";
    }

    dot << "}\n";

    std::filesystem::path outPath(filename);
    std::string           extension = outPath.extension().string();
    if (!extension.empty() && extension[0] == '.')
        extension.erase(0, 1);

    std::string baseName    = outPath.stem().string();
    std::string dotFilename = "temp_sim_" + baseName + ".dot";

    {
        std::ofstream ofs(dotFilename);
        if (!ofs)
            throw std::runtime_error("Failed to write simulation dot file");
        ofs << dot.str();
    }

    std::string settings =
        (extension == "png" || extension == "jpg" || extension == "webp") ? " -Gdpi=300 " : "";
    std::string command = std::string(DOT_PATH) + " -T" + extension + settings + " \"" +
                          dotFilename + "\" -o \"" + filename + "\"";

    if (int result = std::system(command.c_str()); result != 0)
        throw std::runtime_error("Simulation export failed");

    std::cout << "[PDA] Simulation visualization created: " << filename << "\n";
    std::remove(dotFilename.c_str());
}

bool PDA::simulateCykSymbols(const std::vector<std::string>& cykSymbols) {
    // Convert CYK symbols to a string separated by spaces
    std::string input;
    for (const auto& sym : cykSymbols) {
        if (!input.empty())
            input += " ";
        input += sym;
    }
    return simulate(input);
}
