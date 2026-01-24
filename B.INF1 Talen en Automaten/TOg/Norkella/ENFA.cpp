#include "ENFA.h"
#include "General.h"
#include <fstream>
#include "../json.hpp"
#include "../Adriaan/ENFA.h"
namespace Norkella {
using json = nlohmann::json;

std::set<std::string> ENFA::epsilonClosure(const std::set<std::string>& states) {
    std::set<std::string> closure = states;
    std::queue<std::string> queue;

    for (const auto& state : states) {
        queue.push(state);
    }

    while (!queue.empty()) {
        std::string current = queue.front();
        queue.pop();

        auto it = transitionTable.find(current);
        if (it != transitionTable.end()) {
            auto epsilonTrans = it->second.find(epsilon);
            if (epsilonTrans != it->second.end()) {
                for (const auto& state : epsilonTrans->second) {
                    if (closure.insert(state).second) {
                        queue.push(state);
                    }
                }
            }
        }
    }

    return closure;
}

std::set<std::string> ENFA::move(const std::set<std::string>& states, char symbol) {
    std::set<std::string> result;

    for (const auto& state : states) {
        auto it = transitionTable.find(state);
        if (it != transitionTable.end()) {
            auto symbolIt = it->second.find(symbol);
            if (symbolIt != it->second.end()) {
                result.insert(symbolIt->second.begin(), symbolIt->second.end());
            }
        }
    }

    return result;
}

std::string ENFA::getStateName(const std::set<std::string>& states) {
    if (states.empty()) return "{}";

    std::vector<std::string> sortedStates(states.begin(), states.end());
    std::sort(sortedStates.begin(), sortedStates.end());

    std::string result = "{";
    for (size_t i = 0; i < sortedStates.size(); ++i) {
        if (i != 0) result += ",";
        result += sortedStates[i];
    }
    result += "}";

    return result;
}

ENFA::ENFA(const std::string &filename) {
  std::ifstream input(filename);
  if (!input) {
    throw std::runtime_error("Error: Cannot open JSON file: " + filename);
  }

  json j;
  input >> j;

  // Read alphabet
  for (const auto &symbol : j["alphabet"]) {
    alphabet.insert(symbol.get<std::string>()[0]);
  }

  // Read epsilon symbol
  epsilon = j["eps"].get<std::string>()[0];

  // Read states
  for (const auto &state : j["states"]) {
    std::string stateName = state["name"].get<std::string>();
    states.insert(stateName);

    if (state.value("starting", false)) {
      initialState = stateName;
    }
    if (state.value("accepting", false)) {
      acceptingStates.insert(stateName);
    }
  }

  // Read transitions
  for (const auto &transition : j["transitions"]) {
    auto fromState = transition["from"].get<std::string>();
    auto toState = transition["to"].get<std::string>();
    char inputSymbol = transition["input"].get<std::string>()[0];
    transitionTable[fromState][inputSymbol].insert(toState);
  }
}

ENFA::ENFA(const Adriaan::ENFA &enfa) {
    epsilon = enfa.epsilonChar;
    alphabet = enfa.alphabet;
    for (auto state:enfa.states) {
        states.insert(encodeBase64(state));
    }
    for (auto state:enfa.acceptingStates) {
        acceptingStates.insert(encodeBase64(state));
    }
    initialState = encodeBase64(enfa.startState);
    for (const auto& entry : enfa.transitions) {
        int state = entry.first.first;
        char symbol = entry.first.second;
        const std::set<int>& destinationStates = entry.second;

        std::string stateStr = encodeBase64(state);

        for (int destState : destinationStates) {
            transitionTable[stateStr][symbol].insert(encodeBase64(destState));
        }
    }
}


DFA ENFA::toDFA(const bool useSmallNames) {
    DFA dfa;
    dfa.setAlphabet(this->alphabet);

    std::map<std::set<std::string>, std::string> dfaStateNames;
    std::queue<std::set<std::string>> unprocessedStates;
    int stateCounter = 0;

    auto makeName = [&](const std::set<std::string>& stateSet) {
        if (useSmallNames) {
            return encodeBase64(stateCounter++);
        } else {
            return getStateName(stateSet);
        }
    };

    // Initial state is epsilon-closure of the ENFA's initial state
    std::set<std::string> initialENFAState = {initialState};
    std::set<std::string> initialDFAState = epsilonClosure(initialENFAState);
    std::string initialStateName = makeName(initialDFAState);

    // Check if initial state is accepting
    bool isInitialAccepting = false;
    for (const auto& state : initialDFAState) {
        if (acceptingStates.count(state)) {
            isInitialAccepting = true;
            break;
        }
    }

    dfa.addState(initialStateName, true, isInitialAccepting);
    dfaStateNames[initialDFAState] = initialStateName;
    unprocessedStates.push(initialDFAState);

    while (!unprocessedStates.empty()) {
        auto current = unprocessedStates.front();
        unprocessedStates.pop();
        std::string currentName = dfaStateNames[current];

        for (char symbol : alphabet) {
            // Move on symbol followed by epsilon-closure
            auto moved = move(current, symbol);
            auto next = epsilonClosure(moved);

            if (next.empty()) continue;

            std::string nextName;
            if (dfaStateNames.find(next) == dfaStateNames.end()) {
                nextName = makeName(next);

                // Check if next state is accepting
                bool isAccepting = false;
                for (const auto& state : next) {
                    if (acceptingStates.count(state)) {
                        isAccepting = true;
                        break;
                    }
                }

                dfa.addState(nextName, false, isAccepting);
                dfaStateNames[next] = nextName;
                unprocessedStates.push(next);
            } else {
                nextName = dfaStateNames[next];
            }

            dfa.addTransition(currentName, symbol, nextName);
        }
    }

    return dfa;
}

void ENFA::addState(const std::string& name, bool isStarting, bool isAccepting) {
    states.insert(name);
    if (isStarting) {
        initialState = name;
    }
    if (isAccepting) {
        acceptingStates.insert(name);
    }
}
void ENFA::addTransition(const std::string& from, char input, const std::string& to) {
    transitionTable[from][input].insert(to);
}

void ENFA::setAlphabet(const std::set<char>& alphabet) {
    this->alphabet = alphabet;
}

void ENFA::setEpsilon(char eps) {
    epsilon = eps;
}
bool ENFA::accepts(const std::string& input) {
    //Start with the epsilon-eclosure of the initial state
    std::set<std::string> currentStates = epsilonClosure({initialState});
    return acceptsHelper(currentStates, input);
}

bool ENFA::acceptsHelper(const std::set<std::string>& currentStates, const std::string& remainingInput) {
    // Base case: empty input
    if (remainingInput.empty()) {
        // Check if any of the currentStates is an accepting state
        for (const auto& state : currentStates) {
            if (acceptingStates.count(state)) {
                return true;
            }
        }
        return false;
    }

    // Take the first character of the input
    char currentChar = remainingInput[0];
    std::string nextInput = remainingInput.substr(1);

    // Compute all states reachable via the current character
    std::set<std::string> nextStates;
    for (const auto& state : currentStates) {
        auto stateTransitions = transitionTable.find(state);
        if (stateTransitions != transitionTable.end()) {
            auto charTransitions = stateTransitions->second.find(currentChar);
            if (charTransitions != stateTransitions->second.end()) {
                for (const auto& toState : charTransitions->second) {
                    nextStates.insert(toState);
                }
            }
        }
    }

    // Take epsilon-closure of these states
    std::set<std::string> closure = epsilonClosure(nextStates);

    // If no next states, reject
    if (closure.empty()) {
        return false;
    }

    // Recursive call with the new states and remaining input
    return acceptsHelper(closure, nextInput);
}
bool ENFA::isAcceptingState(const std::string& state) const {
    return acceptingStates.count(state) > 0;
}
void ENFA::kleeneStar() {
    std::string newStart = std::to_string(states.size());
    std::string newEnd = std::to_string(states.size() + 1);

    // Add new start and end states
    addState(newStart, true, false);
    addState(newEnd, false, true);

    // Epsilon transitions from new start to old start and new end
    addTransition(newStart, epsilon, initialState);
    addTransition(newStart, epsilon, newEnd);

    // Epsilon transitions from all old accepting states to new end and old start
    for (const auto& state : acceptingStates) {
        addTransition(state, epsilon, initialState);
        addTransition(state, epsilon, newEnd);
    }

    // Update initial en accepting states
    initialState = newStart;
    acceptingStates.clear();
    acceptingStates.insert(newEnd);
}

void ENFA::concatenate(const ENFA& other) {
    // First, modify all accepting states of this
    std::set<std::string> oldAcceptingStates = acceptingStates;
    acceptingStates.clear();

    // For each old accepting state, add epsilon transitions
    // to the initial state of the other
    for (const auto& state : oldAcceptingStates) {
        addTransition(state, epsilon, other.initialState);
    }

    // Add all states from the other (with a suffix to avoid conflicts)
    int offset = states.size();
    std::unordered_map<std::string, std::string> stateMapping;

    for (const auto& state : other.states) {
        std::string newState = state + "_" + std::to_string(offset);
        stateMapping[state] = newState;
        addState(newState, false, other.isAcceptingState(state));
    }

    // Add all transitions from the other
    for (const auto& trans : other.transitionTable) {
        for (const auto& sym : trans.second) {
            for (const auto& toState : sym.second) {
                addTransition(stateMapping[trans.first],
                            sym.first,
                            stateMapping[toState]);
            }
        }
    }

    // Update accepting states with the new states from the other
    for (const auto& state : other.acceptingStates) {
        acceptingStates.insert(stateMapping[state]);
    }

    // Merge alphabets
    alphabet.insert(other.alphabet.begin(), other.alphabet.end());
}

void ENFA::unionWith(const ENFA& other) {
    std::string newStart = "S_" + std::to_string(states.size());
    std::string newEnd = "E_" + std::to_string(states.size() + 1);

    // Add new start and end states
    addState(newStart, true, false);
    addState(newEnd, false, true);

    // Epsilon transitions from new start to both initial states
    addTransition(newStart, epsilon, initialState);
    addTransition(newStart, epsilon, other.initialState);

    // Epsilon transitions from all accepting states to new end
    for (const auto& state : acceptingStates) {
        addTransition(state, epsilon, newEnd);
    }

    // Add all states from 'other' (with a suffix to avoid conflicts)
    int offset = states.size();
    std::unordered_map<std::string, std::string> stateMapping;

    for (const auto& state : other.states) {
        std::string newState = state + "_" + std::to_string(offset);
        stateMapping[state] = newState;
        addState(newState, false, false); // Accepting handled via epsilon transitions

        // Add transitions from old accepting states of 'other' to new end
        if (other.isAcceptingState(state)) {
            addTransition(newState, epsilon, newEnd);
        }
    }

    //Add all transitions from other
    for (const auto& trans : other.transitionTable) {
        for (const auto& sym : trans.second) {
            for (const auto& toState : sym.second) {
                addTransition(stateMapping[trans.first],
                            sym.first,
                            stateMapping[toState]);
            }
        }
    }

    // Update initial en accepting states
    initialState = newStart;
    acceptingStates.clear();
    acceptingStates.insert(newEnd);

    // Merge alphabets
    alphabet.insert(other.alphabet.begin(), other.alphabet.end());
}
void ENFA::printStats() const {
    std::cout << "no_of_states=" << states.size() << std::endl;

    // count transitions by symbool
    std::map<char, int> transitionCounts;
    for (const auto& trans : transitionTable) {
        for (const auto& sym : trans.second) {
            transitionCounts[sym.first] += sym.second.size();
        }
    }

    // Epsilon first
    if (transitionCounts.count(epsilon)) {
        std::cout << "no_of_transitions[" << epsilon << "]="
                  << transitionCounts[epsilon] << std::endl;
        transitionCounts.erase(epsilon);
    }

    //Other symbols in alphabetical order
    for (const auto& sym : transitionCounts) {
        std::cout << "no_of_transitions[" << sym.first << "]="
                  << sym.second << std::endl;
    }

    // Calculate the grade of the states
    std::map<int, int> degreeCounts;
    for (const auto& state : states) {
        int degree = 0;
        auto it = transitionTable.find(state);
        if (it != transitionTable.end()) {
            for (const auto& trans : it->second) {
                degree += trans.second.size();
            }
        }
        degreeCounts[degree]++;
    }

    for (const auto& deg : degreeCounts) {
        std::cout << "degree[" << deg.first << "]=" << deg.second << std::endl;
    }
}
}