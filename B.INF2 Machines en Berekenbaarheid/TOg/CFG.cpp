#include "CFG.h"

#include "config/OperatorConfig.h"
#include "utils/Tokenizer.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

CFG::CFG(const std::string& filename, const bool isFile) {
    if (isFile)
        loadFromJsonFile(filename);
    else
        loadFromJsonString(filename);
}

void CFG::setVariables(const std::set<std::string>& newVariables) { variables = newVariables; }

void CFG::setTerminals(const std::set<std::string>& newTerminals) { terminals = newTerminals; }

void CFG::setStartSymbol(const std::string& newStartSymbol) { startSymbol = newStartSymbol; }

void CFG::setProductions(
    const std::set<std::pair<std::string, std::vector<std::string>>>& newProductions) {
    productions = newProductions;
}

static const cfg::OperatorConfig* g_ops = nullptr;

void                              CFG::setOperatorConfig(const cfg::OperatorConfig* ops) {
    opConfig_ = ops;
    g_ops     = ops;
}

void CFG::addProduction(const std::string& head, const std::vector<std::string>& body) {
    productions.insert({head, body});
}

bool CFG::hasProduction(const std::string& head, const std::vector<std::string>& body) const {
    return productions.contains({head, body});
}

void CFG::loadFromJsonFile(const std::string& filename) {
    std::ifstream input(filename);
    if (!input.is_open())
        throw std::runtime_error("Cannot open file: " + filename);

    json j;
    try {
        input >> j;
        loadFromJSON(j); // Call the new common helper
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parsing error in file " + filename + ": " + e.what());
    }
}

// New method to load directly from a JSON string
void CFG::loadFromJsonString(const std::string& json_string) {
    json j;
    try {
        j = json::parse(json_string); // Parse the string into a json object
        loadFromJSON(j);              // Call the new common helper
    } catch (const json::parse_error& e) {
        throw std::runtime_error("JSON parsing error in string: " + std::string(e.what()));
    }
}

void CFG::loadFromJSON(const json& j) {
    variables.clear();
    terminals.clear();
    productions.clear();

    // All the logic from your original function goes here,
    // using the passed 'j' object

    if (j.contains("Variables") && j["Variables"].is_array()) {
        for (const auto& var : j["Variables"]) {
            if (!var.is_string())
                continue; // Basic type check
            variables.insert(var.get<std::string>());
        }
    }

    if (j.contains("Terminals") && j["Terminals"].is_array()) {
        for (const auto& term : j["Terminals"]) {
            if (!term.is_string())
                continue;
            terminals.insert(term.get<std::string>());
        }
    }

    if (j.contains("Productions") && j["Productions"].is_array()) {
        for (const auto& prod : j["Productions"]) {
            if (prod.contains("head") && prod.contains("body") && prod["head"].is_string()) {
                std::string              head = prod["head"].get<std::string>();
                std::vector<std::string> body;

                if (prod["body"].is_array()) {
                    for (const auto& symbol : prod["body"])
                        if (symbol.is_string())
                            body.push_back(symbol.get<std::string>());
                }

                productions.insert({head, body});
            }
        }
    }

    if (j.contains("Start") && j["Start"].is_string())
        startSymbol = j["Start"].get<std::string>();
}

static bool isNumberLocal(const std::string& s) {
    if (s.empty())
        return false;
    bool hasDecimal = false;

    for (size_t i = 0; i < s.size(); ++i) {
        if (i == 0 && s[i] == '-') {
            if (s.size() == 1)
                return false;
            continue;
        }
        if (s[i] == '.') {
            if (hasDecimal)
                return false;
            hasDecimal = true;
        } else if (!std::isdigit((unsigned char)s[i])) {
            return false;
        }
    }
    return true;
}

/**
 * @brief True if token is a configured call-style operator (FUNC1 or FUNC2).
 *
 * This covers both identifier-like functions (min, max, sin) and symbol-like functions ($),
 * as long as they appear in OperatorConfig as func1/func2.
 */
static bool isFuncLocal(const std::string& t) {
    return (g_ops && (g_ops->isFunc1(t) || g_ops->isFunc2(t)));
}

std::vector<std::string> CFG::preprocessImplicitMul(const std::vector<std::string>& tokens) {
    std::vector<std::string> out;
    out.reserve(tokens.size() * 2);

    for (size_t i = 0; i < tokens.size(); ++i) {
        out.push_back(tokens[i]);

        if (i + 1 < tokens.size()) {
            const std::string& a = tokens[i];
            const std::string& b = tokens[i + 1];

            // Does current token end an "atom"/expression chunk?
            bool currEnds = false;
            if (a == ")")
                currEnds = true;
            else if (isNumberLocal(a))
                currEnds = true;
            else if (g_ops && g_ops->isPostfix(a))
                currEnds = true;

            // Does next token start an "atom"/expression chunk?
            bool currStarts = false;

            if (b == "(") {
                currStarts = true;
            }
            // Start of function call token (min, max, sin, $, ...)
            else if (isFuncLocal(b)) {
                currStarts = true;
            }
            // Numbers can start an atom, but we explicitly block NUM NUM
            else if (isNumberLocal(b)) {
                currStarts = !isNumberLocal(a);
            }

            if (currEnds && currStarts)
                out.push_back("*");
        }
    }
    return out;
}

std::vector<std::string> CFG::preprocessImplicitMulForCYK_NUM_LPAREN(
    const std::vector<std::string>& tokens) {
    std::vector<std::string> out;
    out.reserve(tokens.size() * 2);

    auto isNum = [&](const std::string& t) { return t == "NUM" || isNumberLocal(t); };

    // Helper: treat FUNC1/FUNC2 as atom starters in CYK-token streams
    auto isFuncSym = [&](const std::string& t) { return (t == "FUNC1" || t == "FUNC2"); };

    for (size_t i = 0; i < tokens.size(); ++i) {
        out.push_back(tokens[i]);

        if (i + 1 < tokens.size()) {
            const std::string& a = tokens[i];
            const std::string& b = tokens[i + 1];

            // End of atom in CYK-symbol stream
            bool currEnds = (a == ")" || isNum(a) || a == "OP_UN_POST");

            // Start of atom in CYK-symbol stream
            bool currStarts = false;

            if (b == "(") {
                currStarts = true;
            } else if (isFuncSym(b)) {
                currStarts = true; // NUM FUNC, ) FUNC, etc.
            } else if (isNum(b)) {
                // Block NUM NUM (both can be "NUM" or a literal number)
                currStarts = !isNum(a);
            } else if (b == "OP_UN_PRE") {
                currStarts = true;
            }

            if (currEnds && currStarts)
                out.push_back("OP_BIN");
        }
    }

    return out;
}

void CFG::print() const {
    std::cout << "V = {";
    printSortedSet(variables);
    std::cout << "}" << std::endl;

    std::cout << "T = {";
    printSortedSet(terminals);
    std::cout << "}" << std::endl;

    std::cout << "P = {" << std::endl;
    printProductions();
    std::cout << "}" << std::endl;

    std::cout << "S = " << startSymbol << std::endl;
}

// PRINTING HELPERS

void CFG::printSortedSet(const std::set<std::string>& s) {
    bool first = true;
    for (const auto& element : s) {
        if (!first)
            std::cout << ", ";
        std::cout << element;
        first = false;
    }
}

void CFG::printProductions() const {
    std::map<std::string, std::vector<std::vector<std::string>>> sortedProductions;

    // Group productions by head
    for (const auto& production : productions)
        sortedProductions[production.first].push_back(production.second);

    // Find the maximum head length for alignment (excluding startSymbol)
    size_t maxHeadLength = 0;
    for (const auto& varProds : sortedProductions)
        if (varProds.first != startSymbol && varProds.first.length() > maxHeadLength)
            maxHeadLength = varProds.first.length();
    // Sort productions for each variable
    for (auto& varProds : sortedProductions) {
        std::vector<std::vector<std::string>>& bodies = varProds.second;

        std::sort(bodies.begin(), bodies.end(),
                  [this](const std::vector<std::string>& a, const std::vector<std::string>& b) {
                      // Check body types
                      bool aHasOnlyVars  = !a.empty();
                      bool bHasOnlyVars  = !b.empty();
                      bool aHasTerminals = false;
                      bool bHasTerminals = false;

                      // Check if body contains only variables
                      for (const auto& symbol : a) {
                          if (terminals.count(symbol)) {
                              aHasOnlyVars  = false;
                              aHasTerminals = true;
                          }
                      }

                      for (const auto& symbol : b) {
                          if (terminals.count(symbol)) {
                              bHasOnlyVars  = false;
                              bHasTerminals = true;
                          }
                      }

                      // Category 1: Bodies with ONLY variables (no terminals)
                      if (aHasOnlyVars && bHasOnlyVars) {
                          // Compare symbol by symbol
                          size_t minSize = std::min(a.size(), b.size());
                          for (size_t i = 0; i < minSize; ++i)
                              if (a[i] != b[i])
                                  return a[i] < b[i]; // alphabetical order
                          // If all compared symbols are equal, longer body comes first
                          return a.size() > b.size();
                      }

                      // Category 2: epsilon
                      if (a.empty() && bHasOnlyVars)
                          return false; // epsilon after variables
                      if (a.empty() && bHasTerminals)
                          return true; // epsilon before terminals
                      if (aHasOnlyVars && b.empty())
                          return true; // variables before epsilon
                      if (aHasTerminals && b.empty())
                          return false; // terminals after epsilon

                      // Both are epsilon
                      if (a.empty() && b.empty())
                          return false;

                      // Category 3: Bodies with terminals
                      if (aHasTerminals && bHasTerminals) {
                          // Compare symbol by symbol
                          size_t minSize = std::min(a.size(), b.size());
                          for (size_t i = 0; i < minSize; ++i)
                              if (a[i] != b[i])
                                  return a[i] < b[i]; // alphabetical order
                          // If all compared symbols are equal, longer body comes first
                          return a.size() > b.size();
                      }

                      if (aHasOnlyVars && bHasTerminals)
                          return true; // variables before terminals
                      if (aHasTerminals && bHasOnlyVars)
                          return false; // terminals after variables

                      return false;
                  });
    }

    // Print all productions with aligned arrows (except for startSymbol)
    for (const auto& varProds : sortedProductions) {
        const std::string&                           variable     = varProds.first;
        const std::vector<std::vector<std::string>>& sortedBodies = varProds.second;

        for (const auto& body : sortedBodies) {
            std::cout << "  ";

            // Special handling for start symbol - no alignment
            if (variable == startSymbol) {
                std::cout << variable << "   -> ";
            } else {
                // Other variables - use alignment
                std::cout << std::left << std::setw(maxHeadLength) << variable << " -> ";
            }

            if (body.empty()) {
                std::cout << "``";
            } else {
                std::cout << "`";
                for (size_t i = 0; i < body.size(); ++i) {
                    if (i > 0)
                        std::cout << " ";
                    std::cout << body[i];
                }
                std::cout << "`";
            }
            std::cout << std::endl;
        }
    }
}

// CNF CONVERSION MAIN METHOD

void CFG::toCNF() {
    std::cout << "Original CFG:\n" << std::endl;
    print();
    std::cout << "\n-------------------------------------\n" << std::endl;

    std::cout << " >> Eliminating epsilon productions" << std::endl;
    eliminateEpsilonProductions();

    std::cout << " >> Eliminating unit pairs" << std::endl;
    eliminateUnitProductions();

    std::cout << " >> Eliminating useless symbols" << std::endl;
    eliminateUselessSymbols();

    std::cout << " >> Replacing terminals in bad bodies" << std::endl;
    replaceTerminalsInBadBodies();

    breakLongBodies();

    std::cout << ">>> Result CFG:\n" << std::endl;
    print();
}

// CNF STEP 1: Epsilon Elimination

std::set<std::string> CFG::findNullableSymbols() {
    std::set<std::string> nullable;
    bool                  changed;

    do {
        changed = false;
        for (const auto& prod : productions) {
            const std::string&              head = prod.first;
            const std::vector<std::string>& body = prod.second;

            if (body.empty()) {
                if (nullable.insert(head).second)
                    changed = true;
            } else {
                bool allNullable = true;
                for (const auto& symbol : body) {
                    // Check if the symbol is a variable and is in the nullable set
                    if (variables.count(symbol) && !nullable.count(symbol)) {
                        allNullable = false;
                        break;
                    }
                    // Terminals are not nullable, so no need to check terminals.count(symbol)
                    // If the symbol is a terminal, it will make allNullable false unless it's a
                    // variable.
                    if (terminals.count(symbol)) {
                        allNullable = false;
                        break;
                    }
                }
                if (allNullable && nullable.insert(head).second)
                    changed = true;
            }
        }
    } while (changed);

    return nullable;
}

void CFG::eliminateEpsilonProductions() {
    auto nullable = findNullableSymbols();
    std::cout << "  Nullables are {";
    printSortedSet(nullable);
    std::cout << "}" << std::endl;

    std::set<std::pair<std::string, std::vector<std::string>>> newProductions;

    for (const auto& prod : productions) {
        const std::string&              head = prod.first;
        const std::vector<std::string>& body = prod.second;

        if (body.empty())
            continue;

        std::vector<int> nullablePositions;
        for (int i = 0; i < body.size(); i++)
            if (variables.count(body[i]) && nullable.count(body[i]))
                nullablePositions.push_back(i);

        int n = nullablePositions.size();
        for (int mask = 0; mask < (1 << n); mask++) {
            std::vector<std::string> newBody;

            // Build the new body by skipping symbols based on the mask
            for (int i = 0, j = 0; i < body.size(); i++) {
                bool isNullablePos = false;
                for (int pos : nullablePositions) {
                    if (i == pos) {
                        isNullablePos = true;
                        break;
                    }
                }

                if (isNullablePos) {
                    // If at a nullable position, check the corresponding bit in the mask
                    if (!(mask & (1 << j)))
                        newBody.push_back(body[i]);
                    j++;
                } else {
                    // Not a nullable symbol, always keep it
                    newBody.push_back(body[i]);
                }
            }

            if (!newBody.empty())
                newProductions.insert({head, newBody});
        }
    }

    std::cout << "  Created " << newProductions.size() << " productions, original had "
              << productions.size() << "\n"
              << std::endl;
    productions = newProductions;
    print();
    std::cout << std::endl;
}

// CNF STEP 2: Unit Production Elimination
std::map<std::pair<std::string, std::string>, std::set<std::string>> CFG::findUnitPairs() {
    std::map<std::pair<std::string, std::string>, std::set<std::string>> unitPairs;

    for (const auto& A : variables)
        unitPairs[{A, A}].insert(A);

    bool changed;
    do {
        changed           = false;
        auto currentState = unitPairs; // Work on copy

        for (const auto& pair : currentState) {
            const std::string& A = pair.first.first;
            const std::string& B = pair.first.second;

            for (const auto& prod : productions) {
                if (prod.first == B && prod.second.size() == 1 && variables.count(prod.second[0])) {
                    std::string C = prod.second[0];
                    if (unitPairs.find({A, C}) == unitPairs.end()) {
                        unitPairs[{A, C}].insert(A);
                        changed = true;
                    }
                }
            }
        }
    } while (changed);

    return unitPairs;
}

void CFG::eliminateUnitProductions() {
    auto unitPairs     = findUnitPairs();

    int  unitProdCount = 0;
    for (const auto& prod : productions) {
        if (prod.second.size() == 1 && variables.count(prod.second[0]) &&
            prod.first != prod.second[0]) {
            unitProdCount++;
        }
    }

    std::cout << "  Found " << unitProdCount << " unit productions" << std::endl;

    std::cout << "  Unit pairs: {";
    bool first = true;
    for (const auto& pair : unitPairs) {
        if (!first)
            std::cout << ", ";
        std::cout << "(" << pair.first.first << ", " << pair.first.second << ")";
        first = false;
    }
    std::cout << "}" << std::endl;

    std::set<std::pair<std::string, std::vector<std::string>>> newProductions;

    // Keep all non-unit productions
    for (const auto& prod : productions)
        if (!(prod.second.size() == 1 && variables.count(prod.second[0])))
            newProductions.insert(prod);

    // Add productions from unit pairs
    for (const auto& pair : unitPairs) {
        const std::string& A = pair.first.first;
        const std::string& B = pair.first.second;

        if (A == B)
            continue;

        auto B_productions = getProductionsByHead(B);
        for (const auto& prod : B_productions) {
            // Only add non-unit productions
            if (!(prod.second.size() == 1 && variables.count(prod.second[0])))
                newProductions.insert({A, prod.second});
        }
    }

    std::cout << "  Created " << newProductions.size() << " new productions, original had "
              << productions.size() << "\n"
              << std::endl;
    productions = newProductions;
    print();
    std::cout << std::endl;
}

// CNF STEP 3: Useless Symbol Elimination

std::set<std::string> CFG::findGeneratingSymbols() {
    std::set<std::string> generating = terminals;
    bool                  changed;

    do {
        changed = false;
        for (const auto& prod : productions) {
            const std::string& head = prod.first;
            // Only variables can be added to the generating set
            if (!variables.count(head) || generating.count(head))
                continue;

            const std::vector<std::string>& body          = prod.second;
            bool                            allGenerating = true;
            for (const auto& symbol : body) {
                if (!generating.count(symbol)) {
                    allGenerating = false;
                    break;
                }
            }

            if (allGenerating) {
                generating.insert(head);
                changed = true;
            }
        }
    } while (changed);

    return generating;
}

std::set<std::string> CFG::findReachableSymbols() {
    std::set<std::string>   reachable;
    std::queue<std::string> q;

    reachable.insert(startSymbol);
    q.push(startSymbol);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        for (const auto& prod : productions) {
            if (prod.first == current) {
                for (const auto& symbol : prod.second) {
                    if (reachable.insert(symbol).second) {
                        // Only push variables onto the queue
                        if (variables.count(symbol))
                            q.push(symbol);
                    }
                }
            }
        }
    }

    return reachable;
}

void CFG::eliminateUselessSymbols() {
    // Step 1: Find generating symbols
    auto generating = findGeneratingSymbols();
    std::cout << "  Generating symbols: {";
    printSortedSet(generating);
    std::cout << "}" << std::endl;

    // Step 2: Keep only generating productions
    std::set<std::pair<std::string, std::vector<std::string>>> generatingProductions;
    std::set<std::string>                                      generatingVariables;
    std::set<std::string>                                      generatingTerminals;

    for (const auto& prod : productions) {
        const std::string&              head = prod.first;
        const std::vector<std::string>& body = prod.second;

        // Check if head is generating
        if (generating.count(head)) {
            // Check if all symbols in body are generating
            bool allGenerating = true;
            for (const auto& symbol : body) {
                if (!generating.count(symbol)) {
                    allGenerating = false;
                    break;
                }
            }

            if (allGenerating) {
                generatingProductions.insert(prod);
                generatingVariables.insert(head);

                // Collect terminals from the body
                for (const auto& symbol : body)
                    if (terminals.count(symbol))
                        generatingTerminals.insert(symbol);
            }
        }
    }
    // Step 3: Find reachable symbols from start symbol in the generating productions
    std::set<std::string>   reachable;
    std::queue<std::string> q;

    reachable.insert(startSymbol);
    q.push(startSymbol);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();

        // Check only the generating productions
        for (const auto& prod : generatingProductions) {
            if (prod.first == current) {
                for (const auto& symbol : prod.second) {
                    if (variables.count(symbol) && generatingVariables.count(symbol)) {
                        if (reachable.insert(symbol).second)
                            q.push(symbol);
                    } else if (terminals.count(symbol)) {
                        // Terminals are reachable, but not added in the queue
                        reachable.insert(symbol);
                    }
                }
            }
        }
    }

    std::cout << "  Reachable symbols: {";
    printSortedSet(reachable);
    std::cout << "}" << std::endl;

    // Step 4: Keep only useful symbols (reachable and generating)
    std::set<std::string> useful;
    for (const auto& symbol : reachable)
        if (generating.count(symbol))
            useful.insert(symbol);
    // Also include terminals that appear in useful productions
    for (const auto& symbol : generatingTerminals)
        useful.insert(symbol);

    std::cout << "  Useful symbols: {";
    printSortedSet(useful);
    std::cout << "}" << std::endl;

    // Step 5: Create final useful productions
    std::set<std::pair<std::string, std::vector<std::string>>> usefulProductions;
    std::set<std::string>                                      usefulVariables;
    std::set<std::string>                                      usefulTerminals;

    for (const auto& prod : generatingProductions) {
        // Only keep productions where head is useful
        if (useful.count(prod.first)) {
            bool allUseful = true;
            // Check that all variable symbols in body are useful
            for (const auto& symbol : prod.second) {
                if (variables.count(symbol) && !useful.count(symbol)) {
                    allUseful = false;
                    break;
                }
            }

            if (allUseful) {
                usefulProductions.insert(prod);
                usefulVariables.insert(prod.first);

                // Collect terminals
                for (const auto& symbol : prod.second)
                    if (terminals.count(symbol))
                        usefulTerminals.insert(symbol);
            }
        }
    }

    // Count changes
    int originalVarCount  = variables.size();
    int originalTermCount = terminals.size();
    int originalProdCount = productions.size();

    // Update the grammar
    variables        = usefulVariables;
    terminals        = usefulTerminals;
    productions      = usefulProductions;

    int removedVars  = originalVarCount - variables.size();
    int removedTerms = originalTermCount - terminals.size();
    int removedProds = originalProdCount - productions.size();

    std::cout << "  Removed " << removedVars << " variables, " << removedTerms << " terminals and "
              << removedProds << " productions \n"
              << std::endl;
    print();
    std::cout << std::endl;
}

// CNF STEP 4: Terminal Replacement
void CFG::replaceTerminalsInBadBodies() {
    std::set<std::pair<std::string, std::vector<std::string>>> newProductions;
    std::set<std::string>                                      newVariables;

    // 1st step: find existing variables that produce single terminals
    std::map<std::string, std::string> terminalToExistingVar;
    for (const auto& prod : productions)
        if (prod.second.size() == 1 && terminals.count(prod.second[0]))
            terminalToExistingVar[prod.second[0]] = prod.first;

    // 2nd step: identify which terminals need replacement and create new variables if needed
    std::set<std::string>              terminalsToReplace;
    std::map<std::string, std::string> terminalToVar;

    for (const auto& prod : productions) {
        const std::vector<std::string>& body = prod.second;

        // Only consider bodies with length > 1 that contain terminals
        if (body.size() > 1) {
            for (const auto& symbol : body) {
                if (terminals.count(symbol)) {
                    terminalsToReplace.insert(symbol);

                    // If we don't already have a mapping for this terminal, create one
                    if (terminalToVar.find(symbol) == terminalToVar.end()) {
                        // Check if there's already a variable that produces this terminal
                        if (terminalToExistingVar.find(symbol) != terminalToExistingVar.end()) {
                            // Reuse existing variable
                            terminalToVar[symbol] = terminalToExistingVar[symbol];
                        } else {
                            // Create new variable
                            std::string newVar    = "_" + symbol;
                            terminalToVar[symbol] = newVar;
                            newVariables.insert(newVar);
                            // Add production: newVar -> terminal
                            newProductions.insert({newVar, {symbol}});
                        }
                    }
                }
            }
        }
    }

    // 3rd step: replace terminals in bodies with length > 1
    for (const auto& prod : productions) {
        const std::string&              head = prod.first;
        const std::vector<std::string>& body = prod.second;

        // Skip productions that are single terminal productions (we already handled them)
        if (body.size() == 1 && terminals.count(body[0])) {
            const std::string& t = body[0];

            // Als deze terminal NIET vervangen moet worden, gewoon behouden.
            if (terminalToVar.find(t) == terminalToVar.end()) {
                newProductions.insert(prod);
            } else {
                // Als hij wél vervangen werd, enkel de gekozen var behouden.
                if (terminalToVar[t] == head)
                    newProductions.insert(prod);
            }
            continue;
        }

        // For other productions, replace terminals where needed
        if (body.size() > 1) {
            bool needsReplacement = false;
            for (const auto& symbol : body) {
                if (terminalsToReplace.count(symbol)) {
                    needsReplacement = true;
                    break;
                }
            }

            if (needsReplacement) {
                std::vector<std::string> newBody;
                for (const auto& symbol : body)
                    if (terminalsToReplace.count(symbol))
                        newBody.push_back(terminalToVar[symbol]);
                    else
                        newBody.push_back(symbol);
                newProductions.insert({head, newBody});
            } else {
                newProductions.insert(prod);
            }
        } else {
            newProductions.insert(prod);
        }
    }

    // Add new variables to the grammar
    variables.insert(newVariables.begin(), newVariables.end());

    std::cout << "  Added " << newVariables.size() << " new variables: {";
    printSortedSet(newVariables);
    std::cout << "}" << std::endl;
    std::cout << "  Created " << newProductions.size() << " new productions, original had "
              << productions.size() << "\n"
              << std::endl;

    productions = newProductions;
    print();
    std::cout << std::endl;
}

// CNF STEP 5: Breaking Long Bodies
std::string CFG::getNewVariable(const std::string& base) {
    int         counter = 2;
    std::string newVar  = base + "_" + std::to_string(counter);
    while (variables.count(newVar)) {
        counter++;
        newVar = base + "_" + std::to_string(counter);
    }
    return newVar;
}

void CFG::breakLongBodies() {
    std::set<std::pair<std::string, std::vector<std::string>>> newProductions;
    int                                                        bodiesBroken = 0;
    int                                                        newVarsAdded = 0;

    for (const auto& prod : productions) {
        const std::string&              head = prod.first;
        const std::vector<std::string>& body = prod.second;

        // CNF requires bodies of length 1 (terminals) or 2 (variables)
        if (body.size() == 1 && terminals.count(body[0])) {
            // Single terminal - keep as is
            newProductions.insert(prod);
        } else if (body.size() == 2) {
            // Two symbols - keep as is (already in CNF)
            newProductions.insert(prod);
        } else if (body.size() > 2) {
            // Break long bodies into pairs
            bodiesBroken++;
            std::vector<std::string> currentBody = body;
            std::string              currentHead = head;

            while (currentBody.size() > 2) {
                // Use the current head as base for new variables
                std::string newVar = getNewVariable(currentHead);
                variables.insert(newVar);
                newVarsAdded++;

                // Create new production: currentHead -> firstSymbol newVar
                std::vector<std::string> newProdBody;
                newProdBody.push_back(currentBody[0]);
                newProdBody.push_back(newVar);

                newProductions.insert({currentHead, newProdBody});

                // Continue with the rest of the body
                currentHead = newVar;
                currentBody.erase(currentBody.begin());
            }

            // Add the final production with 2 symbols
            newProductions.insert({currentHead, currentBody});
        } else {
            // Empty production (shouldn't happen after epsilon elimination)
            newProductions.insert(prod);
        }
    }

    std::cout << " >> Broke " << bodiesBroken << " bodies, added " << newVarsAdded
              << " new variables" << std::endl;
    productions = newProductions;
}

std::vector<std::pair<std::string, std::vector<std::string>>> CFG::getProductionsByHead(
    const std::string& head) const {
    std::vector<std::pair<std::string, std::vector<std::string>>> result;
    for (const auto& prod : productions)
        if (prod.first == head)
            result.push_back(prod);
    return result;
}

bool CFG::accepts(const std::string& input) {
    // Backward-compatible wrapper: treat each character as a separate token.
    std::vector<std::string> tokens;
    tokens.reserve(input.size());
    for (char c : input)
        tokens.emplace_back(1, c);
    return acceptsTokens(tokens);
}

bool CFG::acceptsTokens(const std::vector<std::string>& tokens) {
    const int n = static_cast<int>(tokens.size());

    if (n == 0) {
        const auto nullable = findNullableSymbols();
        bool       accepted = nullable.count(startSymbol) > 0;
        std::cout << (accepted ? "true" : "false") << std::endl;
        return accepted;
    }

    // Precompute lookups
    std::map<std::string, std::set<std::string>>                         terminalToHeads;
    std::map<std::pair<std::string, std::string>, std::set<std::string>> pairToHeads;

    for (const auto& prod : productions)
        if (prod.second.size() == 1)
            terminalToHeads[prod.second[0]].insert(prod.first);
        else if (prod.second.size() == 2)
            pairToHeads[{prod.second[0], prod.second[1]}].insert(prod.first);

    std::vector<std::vector<std::set<std::string>>> table(n, std::vector<std::set<std::string>>(n));

    for (int i = 0; i < n; ++i) {
        auto it = terminalToHeads.find(tokens[i]);
        if (it != terminalToHeads.end())
            table[i][0] = it->second;
    }

    for (int length = 2; length <= n; ++length) {
        for (int start = 0; start <= n - length; ++start) {
            for (int split = 1; split < length; ++split) {
                const auto& leftSet  = table[start][split - 1];
                const auto& rightSet = table[start + split][length - split - 1];

                for (const auto& B : leftSet) {
                    for (const auto& C : rightSet) {
                        auto pit = pairToHeads.find({B, C});
                        if (pit != pairToHeads.end())
                            table[start][length - 1].insert(pit->second.begin(), pit->second.end());
                    }
                }
            }
        }
    }

    displayCYKTable(table, tokens);

    bool accepted = table[0][n - 1].count(startSymbol) > 0;
    std::cout << (accepted ? "true" : "false") << std::endl;
    return accepted;
}

void CFG::displayCYKTable(const std::vector<std::vector<std::set<std::string>>>& table,
                          const std::string&                                     input) {
    int n = input.length();

    // Print table - row i corresponds to substrings of length n-i
    for (int displayRow = 0; displayRow < n; displayRow++) {
        int length = n - displayRow; // Actual substring length

        // Print opening |
        std::cout << "| ";

        // Print cells for this row with proper spacing
        for (int start = 0; start <= n - length; start++) {
            const auto& cell = table[start][length - 1];

            // Print the cell content
            std::cout << "{";
            bool first = true;
            for (const auto& var : cell) {
                if (!first)
                    std::cout << ", ";
                std::cout << var;
                first = false;
            }
            std::cout << "}";

            // Add separator if not the last cell in row
            if (start < n - length)
                std::cout << " | ";
        }

        // Print closing |
        std::cout << " |";
        std::cout << std::endl;
    }
}

void CFG::displayCYKTable(const std::vector<std::vector<std::set<std::string>>>& table,
                          const std::vector<std::string>&                        tokens) {
    const int n = static_cast<int>(tokens.size());

    for (int displayRow = 0; displayRow < n; ++displayRow) {
        int length = n - displayRow;
        std::cout << "| ";

        for (int start = 0; start <= n - length; ++start) {
            const auto& cell = table[start][length - 1];

            std::cout << "{";
            bool first = true;
            for (const auto& var : cell) {
                if (!first)
                    std::cout << ", ";
                std::cout << var;
                first = false;
            }
            std::cout << "}";

            if (start < n - length)
                std::cout << " | ";
        }
        std::cout << " |" << std::endl;
    }
}
