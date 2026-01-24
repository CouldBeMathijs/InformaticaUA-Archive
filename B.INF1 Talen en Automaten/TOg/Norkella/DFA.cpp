#include "DFA.h"
#include "General.h"
#include "RE.h"
#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <unordered_set>
#include <vector>

#ifdef _WIN32
const std::string DOT_PATH = "\"C:\\Program Files\\Graphviz\\bin\\dot.exe\"";
#else
const std::basic_string DOT_PATH = "dot";
#endif

namespace Norkella {
auto compressLabels =
    [](const std::vector<std::string> &symbols) -> std::string {
  std::set<char> chars;
  for (const auto &s : symbols) {
    if (s.size() == 1)
      chars.insert(s[0]);
  }

  std::vector<std::string> parts;

  auto extractRange = [&](char start, char end, const std::string &label) {
    bool hasFullRange = true;
    for (char c = start; c <= end; ++c) {
      if (!chars.count(c)) {
        hasFullRange = false;
        break;
      }
    }

    if (hasFullRange) {
      parts.push_back(label);
      for (char c = start; c <= end; ++c) {
        chars.erase(c); // remove so we don't repeat
      }
    }
  };

  extractRange('a', 'z', "[a–z]");
  extractRange('A', 'Z', "[A–Z]");
  extractRange('0', '9', "[0–9]");

  // Add remaining single characters
  for (char c : chars) {
    if (c == '"')
      parts.emplace_back("\\\""); // escape quote
    if (c == ' ')
      parts.emplace_back("[space]");
    else
      parts.emplace_back(1, c);
  }

  return std::accumulate(parts.begin(), parts.end(), std::string(),
                         [](const std::string &a, const std::string &b) {
                           return a.empty() ? b : a + ", " + b;
                         });
};
using namespace std;
DFA::DFA(const std::string &filename) {
  std::ifstream input(filename);
  if (!input) {
    throw std::runtime_error("Error: Cannot open JSON file: " + filename);
  }

  json j;
  input >> j;
  loadFromJSON(j);
}
void DFA::loadFromJSON(const json &j) {
  // Read alphabet
  for (const auto &symbol : j["alphabet"]) {
    alphabet.insert(symbol.get<std::string>()[0]); // Convert string to char
  }

  // Read states
  for (const auto &state : j["states"]) {
    std::string stateName = state["name"].get<std::string>(); // Get state name
    states.insert(stateName);

    if (state.value("starting", false)) {
      initialState = stateName; // Store the initial state name
    }
    if (state.value("accepting", false)) {
      acceptingStates.insert(stateName); // Store accepting state name
    }
  }

  // Read transitions
  for (const auto &transition : j["transitions"]) {
    std::string fromState = transition["from"].get<std::string>();
    std::string toState = transition["to"].get<std::string>();
    char inputSymbol = transition["input"].get<std::string>()[0];
    transitionTable[fromState][inputSymbol] = toState;
  }
}
bool DFA::accepts(const std::string &input) {
  std::string currentState = initialState;

  for (char symbol : input) {
    // Check if the symbol is in the alphabet
    if (alphabet.find(symbol) == alphabet.end()) {
      return false; // Symbol not in alphabet, string is not accepted
    }

    // Look for the next state using the transition table
    auto stateTransitions = transitionTable.find(currentState);
    if (stateTransitions == transitionTable.end()) {
      return false; // No transitions defined for the current state
    }

    auto nextState = stateTransitions->second.find(symbol);
    if (nextState == stateTransitions->second.end()) {
      return false; // No transition defined for the current state and symbol
    }

    currentState = nextState->second;
  }
  // Check if the final state is an accepting state
  return acceptingStates.find(currentState) != acceptingStates.end();
}
void DFA::print() {
  json j;
  j["type"] = "DFA";
  for (char symbol : alphabet) {
    j["alphabet"].push_back(std::string(1, symbol));
  }
  // Convert states
  for (const std::string &stateName : states) {
    j["states"].push_back(
        {{"name", stateName},
         {"starting", stateName == initialState},
         {"accepting", acceptingStates.count(stateName) > 0}});
  }

  // Convert transitions
  for (const auto &[fromState, transitions] : transitionTable) {
    for (const auto &[input, toState] : transitions) {
      j["transitions"].push_back({{"from", fromState},
                                  {"to", toState},
                                  {"input", std::string(1, input)}});
    }
  }
  std::cout << std::setw(4) << j << std::endl;
}
void DFA::setAlphabet(const std::set<char> &alphabet) {
  this->alphabet = alphabet;
}

void DFA::addState(const std::string &name, bool isStarting, bool isAccepting) {
  states.insert(name);
  if (isStarting) {
    initialState = name;
  }
  if (isAccepting) {
    acceptingStates.insert(name);
  }
}

void DFA::addTransition(const std::string &from, char input,
                        const std::string &to) {
  transitionTable[from][input] = to;
}
std::string DFA::getTransition(const std::string &from, char input) const {
  return transitionTable.at(from).at(input);
}

DFA::DFA(const DFA &dfa1, const DFA &dfa2, bool intersection) {
  // Check if the alfabet is the same for both
  if (dfa1.getAlphabet() != dfa2.getAlphabet()) {
    throw std::runtime_error("DFA's have different alphabets");
  }
  alphabet = dfa1.getAlphabet();

  std::map<std::pair<std::string, std::string>, std::string> productStates;
  std::queue<std::pair<std::string, std::string>> unprocessedStates;

  // Initial state
  auto initialState =
      std::make_pair(dfa1.getInitialState(), dfa2.getInitialState());
  std::string initialStateName =
      "(" + initialState.first + "," + initialState.second + ")";

  // Determine whether the initial state is accepting
  bool isInitialAccepting = intersection
                                ? (dfa1.isAcceptingState(initialState.first) &&
                                   dfa2.isAcceptingState(initialState.second))
                                : (dfa1.isAcceptingState(initialState.first) ||
                                   dfa2.isAcceptingState(initialState.second));

  addState(initialStateName, true, isInitialAccepting);
  productStates[initialState] = initialStateName;
  unprocessedStates.push(initialState);

  while (!unprocessedStates.empty()) {
    auto current = unprocessedStates.front();
    unprocessedStates.pop();
    std::string currentName = productStates[current];

    for (char symbol : alphabet) {
      // Find transitions in both DFAs
      const auto &transitions1 = dfa1.getTransitions(current.first);
      const auto &transitions2 = dfa2.getTransitions(current.second);

      auto it1 = transitions1.find(symbol);
      auto it2 = transitions2.find(symbol);
      std::string next1 = it1 != transitions1.end() ? it1->second : current.first;
      std::string next2 = it2 != transitions2.end() ? it2->second : current.second;
      auto next = std::make_pair(next1, next2);

      std::string nextName;

      if (productStates.find(next) == productStates.end()) {
        nextName = "(" + next.first + "," + next.second + ")";

        // Determine whether the next state is accepting
        bool isAccepting = intersection ? (dfa1.isAcceptingState(next.first) &&
                                           dfa2.isAcceptingState(next.second))
                                        : (dfa1.isAcceptingState(next.first) ||
                                           dfa2.isAcceptingState(next.second));

        addState(nextName, false, isAccepting);
        productStates[next] = nextName;
        unprocessedStates.push(next);
      } else {
        nextName = productStates[next];
      }

      addTransition(currentName, symbol, nextName);
    }
  }
}
const std::set<char> &DFA::getAlphabet() const { return alphabet; }

const std::string &DFA::getInitialState() const { return initialState; }

bool DFA::isAcceptingState(const std::string &state) const {
  return acceptingStates.count(state) > 0;
}

const std::map<char, std::string> &
DFA::getTransitions(const std::string &state) const {
  static const std::map<char, std::string> emptyTransitions;
  auto it = transitionTable.find(state);
  return it != transitionTable.end() ? it->second : emptyTransitions;
}
namespace {
std::string getStateName(const std::set<std::string> &states) {
  if (states.empty())
    return "{}";
  std::vector<std::string> sortedStates(states.begin(), states.end());
  std::sort(sortedStates.begin(), sortedStates.end());
  std::string result = "{";
  for (size_t i = 0; i < sortedStates.size(); ++i) {
    if (i != 0)
      result += ", ";
    result += sortedStates[i];
  }
  result += "}";
  return result;
}
} // namespace
const std::set<std::string> &DFA::getStates() const { return states; }

DFA DFA::minimize(const bool smallStateNames) const {
  std::vector<std::string> stateList(states.begin(), states.end());
  std::sort(stateList.begin(), stateList.end());
  int n = static_cast<int>(stateList.size());

  std::unordered_map<std::string, int> stateToIndex;
  for (int i = 0; i < n; ++i)
    stateToIndex[stateList[i]] = i;

  // Precompute: transitions[i][symbol] = index of target state
  std::vector<std::unordered_map<char, int>> transitions(n);
  for (int i = 0; i < n; ++i) {
    const auto &s = stateList[i];
    auto it = transitionTable.find(s);
    if (it != transitionTable.end()) {
      for (const auto &[sym, toState] : it->second) {
        transitions[i][sym] = stateToIndex[toState];
      }
    }
  }

  std::vector<std::vector<bool>> distinguishable(n,
                                                 std::vector<bool>(n, false));

  std::cout << "Step1: Initialize distinguishable table\n";

  // Step 1: Mark pairs where one is accepting and the other is not
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (isAcceptingState(stateList[i]) != isAcceptingState(stateList[j])) {
        distinguishable[i][j] = true;
      }
    }
  }

  std::cout << "Step1: Begin refinement\n";

  // Step 2: Refine
  int iteration = 0;
  const int maxIterations = 100;
  bool changed;

  do {
    changed = false;
    iteration++;
    if (iteration % 10 == 0)
      std::cout << "  Refinement iteration: " << iteration << "\n";
    if (iteration > maxIterations) {
      std::cerr << "Stopped refinement after " << maxIterations
                << " iterations (failsafe)\n";
      break;
    }

    int totalPairs = n * (n - 1) / 2;
    if (totalPairs == 0) {
      std::cout << "DFA has fewer than 2 states. Minimization is skipped.\n";
      return *this;
    }

    int processedPairs = 0;
    int markedPairs = 0;
    int progressStep = std::max(1, totalPairs / 20);

    for (int i = 0; i < n; ++i) {
      for (int j = i + 1; j < n; ++j) {
        processedPairs++;

        if (processedPairs % progressStep == 0) {
          int percent = (processedPairs * 100) / totalPairs;
          std::cout << "Pass " << iteration << " → " << percent
                    << "% complete: marking " << totalPairs << std::endl;
        }

        if (distinguishable[i][j])
          continue;

        for (char symbol : alphabet) {
          bool hasP = transitions[i].count(symbol);
          bool hasQ = transitions[j].count(symbol);

          if (hasP != hasQ) {
            distinguishable[i][j] = true;
            changed = true;
            break;
          }

          if (hasP && hasQ) {
            int pNext = transitions[i][symbol];
            int qNext = transitions[j][symbol];
            if (pNext > qNext)
              std::swap(pNext, qNext);
            if (distinguishable[pNext][qNext]) {
              distinguishable[i][j] = true;
              changed = true;
              break;
            }
          }
        }
      }
    }
  } while (changed);

  std::cout << "Step2: Refinement done, building equivalence classes"
            << std::endl;

  // Step 3: Union-Find
  std::vector<int> parent(n);
  for (int i = 0; i < n; ++i)
    parent[i] = i;

  std::function<int(int)> find = [&](int x) {
    return parent[x] == x ? x : parent[x] = find(parent[x]);
  };

  auto unite = [&](int a, int b) {
    int ra = find(a), rb = find(b);
    if (ra != rb)
      parent[rb] = ra;
  };

  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      if (!distinguishable[i][j]) {
        unite(i, j);
      }
    }
  }

  // Group states by root
  std::unordered_map<int, std::set<std::string>> groups;
  for (int i = 0; i < n; ++i) {
    groups[find(i)].insert(stateList[i]);
  }

  std::cout << "Step3: Creating minimized DFA" << std::endl;

  DFA result;
  result.setAlphabet(alphabet);
  std::map<std::set<std::string>, std::string> groupNames;

  for (const auto &[_, group] : groups) {
    std::string name = getStateName(group);
    bool isInit = group.count(initialState);
    bool isAccept =
        std::any_of(group.begin(), group.end(),
                    [&](const std::string &s) { return isAcceptingState(s); });
    result.addState(name, isInit, isAccept);
    groupNames[group] = name;
  }

  for (const auto &[_, group] : groups) {
    std::string rep = *group.begin();
    std::string from = groupNames[group];
    int repIdx = stateToIndex[rep];

    for (char symbol : alphabet) {
      if (!transitions[repIdx].count(symbol))
        continue;
      int toIdx = transitions[repIdx][symbol];
      const std::string &toState = stateList[toIdx];

      for (const auto &[__, gStates] : groups) {
        if (gStates.count(toState)) {
          result.addTransition(from, symbol, groupNames[gStates]);
          break;
        }
      }
    }
  }

  std::cout << "Minimization complete..." << std::endl;

  if (smallStateNames) {
    result.smallerStateNames();
    std::cout << "Made state names significantly smaller" << std::endl;
  }
  return result;
}

void DFA::smallerStateNames() {
  std::map<std::string, std::string> stateRenameMap;
  std::set<std::string> newStates;
  std::map<std::string, std::map<char, std::string>> newTransitionTable;
  std::unordered_set<std::string> newAcceptingStates;

  // Create a mapping from old state names to new names (S0, S1, ...)
  int index = 0;
  for (const auto &state : states) {
    std::string newName = encodeBase64(index++);
    stateRenameMap[state] = newName;
    newStates.insert(newName);
  }

  // Update the transition table
  for (const auto &[fromState, transitions] : transitionTable) {
    std::string newFrom = stateRenameMap[fromState];
    for (const auto &[symbol, toState] : transitions) {
      std::string newTo = stateRenameMap[toState];
      newTransitionTable[newFrom][symbol] = newTo;
    }
  }

  // Update initial state
  initialState = stateRenameMap[initialState];

  // Update accepting states
  for (const auto &accState : acceptingStates) {
    newAcceptingStates.insert(stateRenameMap[accState]);
  }

  // Assign updated structures back to the DFA
  states = std::move(newStates);
  transitionTable = std::move(newTransitionTable);
  acceptingStates = std::move(newAcceptingStates);
}

void DFA::printTable() const {
  std::vector<std::string> stateList(states.begin(), states.end());
  std::sort(stateList.begin(), stateList.end());

  std::set<std::pair<std::string, std::string>> distinguishablePairs;

  size_t n = stateList.size();
  std::map<std::string, int> indexMap;
  for (size_t i = 0; i < n; ++i) {
    indexMap[stateList[i]] = i;
  }
  // Initial markering: accepterend vs niet-accepterend
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      if (isAcceptingState(stateList[i]) != isAcceptingState(stateList[j])) {
        distinguishablePairs.insert({stateList[i], stateList[j]});
      }
    }
  }

  // Iteratief markeren op basis van transitieve regels
  bool changed;
  do {
    changed = false;
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        const std::string &p = stateList[i];
        const std::string &q = stateList[j];

        if (distinguishablePairs.count({p, q}) ||
            distinguishablePairs.count({q, p}))
          continue;

        for (char symbol : alphabet) {
          const std::string &pNext = transitionTable.at(p).at(symbol);
          const std::string &qNext = transitionTable.at(q).at(symbol);
          auto nextPair = (pNext < qNext) ? std::make_pair(pNext, qNext)
                                          : std::make_pair(qNext, pNext);
          if (distinguishablePairs.count(nextPair)) {
            distinguishablePairs.insert({p, q});
            changed = true;
            break;
          }
        }
      }
    }
  } while (changed);

  // Print driehoekige tabel
  for (size_t i = 1; i < n; ++i) {
    std::cout << i << "\t";
    for (size_t j = 0; j < i; ++j) {
      const std::string &s1 = stateList[j];
      const std::string &s2 = stateList[i];
      auto pair = (s1 < s2) ? std::make_pair(s1, s2) : std::make_pair(s2, s1);
      std::cout << (distinguishablePairs.count(pair) ? "X" : "-") << "\t";
    }
    std::cout << std::endl;
  }

  // Print kolomnummers
  std::cout << "\t";
  for (size_t i = 0; i < n - 1; ++i) {
    std::cout << i << "\t";
  }
  std::cout << std::endl;
}

bool operator==(const DFA &dfa1, const DFA &dfa2) {
  // Combine all states
  std::set<std::string> allStates = dfa1.getStates();
  allStates.insert(dfa2.getStates().begin(), dfa2.getStates().end());

  std::vector<std::string> stateList(allStates.begin(), allStates.end());
  std::sort(stateList.begin(), stateList.end());

  size_t n = stateList.size();
  std::set<std::pair<std::string, std::string>> distinguishablePairs;

  // Initial marking: based on accepting or not
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = i + 1; j < n; ++j) {
      bool accepting1 = dfa1.isAcceptingState(stateList[i]) ||
                        dfa2.isAcceptingState(stateList[i]);
      bool accepting2 = dfa1.isAcceptingState(stateList[j]) ||
                        dfa2.isAcceptingState(stateList[j]);
      if (accepting1 != accepting2) {
        distinguishablePairs.insert({stateList[i], stateList[j]});
      }
    }
  }

  // Apply transitive rules
  bool changed;
  do {
    changed = false;
    for (size_t i = 0; i < n; ++i) {
      for (size_t j = i + 1; j < n; ++j) {
        const std::string &p = stateList[i];
        const std::string &q = stateList[j];
        auto pair = std::make_pair(std::min(p, q), std::max(p, q));
        if (distinguishablePairs.count(pair))
          continue;

        for (char symbol : dfa1.getAlphabet()) {
          std::string pNext, qNext;
          try {
            pNext = dfa1.getTransitions(p).at(symbol);
          } catch (...) {
            try {
              pNext = dfa2.getTransitions(p).at(symbol);
            } catch (...) {
              continue;
            }
          }
          try {
            qNext = dfa1.getTransitions(q).at(symbol);
          } catch (...) {
            try {
              qNext = dfa2.getTransitions(q).at(symbol);
            } catch (...) {
              continue;
            }
          }

          auto nextPair =
              std::make_pair(std::min(pNext, qNext), std::max(pNext, qNext));
          if (distinguishablePairs.count(nextPair)) {
            distinguishablePairs.insert(pair);
            changed = true;
            break;
          }
        }
      }
    }
  } while (changed);

  // print TABEL
  for (size_t i = 1; i < n; ++i) {
    std::cout << stateList[i] << "\t";
    for (size_t j = 0; j < i; ++j) {
      auto pair = std::make_pair(std::min(stateList[i], stateList[j]),
                                 std::max(stateList[i], stateList[j]));
      std::cout << (distinguishablePairs.count(pair) ? "X" : "-") << "\t";
    }
    std::cout << std::endl;
  }

  std::cout << "\t";
  for (size_t i = 0; i < n - 1; ++i) {
    std::cout << stateList[i] << "\t";
  }
  std::cout << std::endl;

  // Check equality via initial states
  std::string s1 = dfa1.getInitialState();
  std::string s2 = dfa2.getInitialState();
  auto startPair = std::make_pair(std::min(s1, s2), std::max(s1, s2));
  return !distinguishablePairs.count(startPair);
}
RE DFA::toRE() const {
  using namespace std;

  vector<string> orderedStates(states.begin(), states.end());
  sort(orderedStates.begin(), orderedStates.end());

  map<string, map<string, RE>> R;

  // Initialize with direct transitions
  for (const auto &fromPair : transitionTable) {
    const string &from = fromPair.first;
    for (const auto &[symbol, to] : fromPair.second) {
      string key = string(1, symbol);
      if (R[from][to].str().empty()) {
        R[from][to] = RE(key);
      } else {
        R[from][to] = RE::unionRE(R[from][to], RE(key));
      }
    }
  }

  string start = initialState;
  set<string> acceptingSet;
  for (const string &s : orderedStates) {
    if (isAcceptingState(s))
      acceptingSet.insert(s);
  }

  // State elimination method
  for (const string &state : orderedStates) {
    if (state == start || acceptingSet.count(state))
      continue;

    for (const string &i : orderedStates) {
      if (i == state)
        continue;
      for (const string &j : orderedStates) {
        if (j == state)
          continue;

        RE Rij = R[i][j];
        RE Rik = R[i][state];
        RE Rkk = R[state][state];
        RE Rkj = R[state][j];

        if (!Rik.str().empty() && !Rkj.str().empty()) {
          RE part = RE::concat(Rik, RE::concat(RE::star(Rkk), Rkj));
          if (Rij.str().empty()) {
            R[i][j] = part;
          } else {
            R[i][j] = RE::unionRE(Rij, part);
          }
        }
      }
    }

    // Clean up eliminated state entries
    for (auto &entry : R) {
      entry.second.erase(state);
    }
    R.erase(state);
  }

  RE finalRE;

  // for each accepting state
  for (const string &accept : acceptingSet) {
    RE pathToAccept = R[start][accept];

    if (!pathToAccept.str().empty()) {
      // Search for possible loops from accepting states to other states
      RE loopFromAccept;
      bool hasLoop = false;

      for (const string &s : orderedStates) {
        if (R[accept][s].str().empty())
          continue;

        hasLoop = true;
        loopFromAccept = loopFromAccept.str().empty()
                             ? R[accept][s]
                             : RE::unionRE(loopFromAccept, R[accept][s]);
      }

      RE expr = hasLoop ? RE::concat(pathToAccept, RE::star(loopFromAccept))
                        : pathToAccept;
      finalRE = finalRE.str().empty() ? expr : RE::unionRE(finalRE, expr);
    }
  }

  return finalRE;
}

void DFA::toImage(const std::string &filename, const std::string &extension) {
  static const std::unordered_set<std::string> supported = {"png", "jpg",
                                                            "webp", "svg"};
  if (!supported.count(extension)) {
    throw std::invalid_argument("Unsupported extension: " + extension);
  }

  // Shorten long state names with a map: original name -> short ID
  std::unordered_map<std::string, std::string> stateIds;
  int stateCounter = 0;

  auto getStateId = [&](const std::string &state) -> std::string {
    auto it = stateIds.find(state);
    if (it != stateIds.end())
      return it->second;

    std::string id = "Q" + std::to_string(stateCounter++);
    stateIds[state] = id;
    return id;
  };

  auto escapeLabel = [](const std::string &label) -> std::string {
    std::string escaped;
    for (char c : label) {
      if (c == '\\' || c == '"')
        escaped += '\\';
      escaped += c;
    }
    return escaped;
  };

  std::stringstream dot;
  dot << "digraph DFA {\n";
  dot << "  bgcolor=\"#282832\";\n";
  dot << "  fontcolor=white;\n";
  dot << "  rankdir=LR;\n";
  dot << "  ranksep=1.5;\n";
  dot << "  nodesep=1.0;\n";
  dot << "  splines=true;\n";
  dot << "  overlap=false;\n";
  dot << "  concentrate=true;\n";
  dot << "  node [shape=circle, width=1, style=filled, fillcolor=\"#141414\", "
         "color=lightgray, fontcolor=white, margin=0.2];\n"; // added margin
  dot << "  edge [arrowsize=0.8, color=lightgray, fontcolor=white, "
         "headclip=true];\n";
  dot << "  __start [shape=point, color=lightgray];\n";
  dot << "  __start -> \"" << getStateId(initialState) << "\";\n";

  std::unordered_set<std::string> renderedStates;
  std::map<std::pair<std::string, std::string>, std::vector<std::string>>
      mergedEdges;

  for (const auto &state : acceptingStates) {
    std::string id = getStateId(state);
    if (renderedStates.insert(id).second)
      dot << "  \"" << id << "\" [shape=doublecircle];\n";
  }

  for (const auto &[fromState, transitions] : transitionTable) {
    std::string fromId = getStateId(fromState);
    if (renderedStates.insert(fromId).second)
      dot << "  \"" << fromId << "\" [shape=circle];\n";

    for (const auto &[symbol, toState] : transitions) {
      std::string toId = getStateId(toState);
      if (renderedStates.insert(toId).second)
        dot << "  \"" << toId << "\" [shape=circle];\n";

      mergedEdges[{fromId, toId}].push_back(
          escapeLabel(std::string(1, symbol)));
    }
  }

  for (const auto &[edge, labels] : mergedEdges) {
    const auto &[from, to] = edge;
    std::string label = compressLabels(labels); // Assume this is efficient
    dot << "  \"" << from << "\" -> \"" << to << "\" [label=\"" << label
        << "\"];\n";
  }

  dot << "}\n";

  std::filesystem::path outPath(filename);
  std::string baseName = outPath.stem().string(); // "abc"
  std::string dotFilename = "temp_dfa_" + baseName + ".dot";
  std::ofstream ofs(dotFilename);
  if (!ofs)
    throw std::runtime_error("Failed to write temporary dot file.");
  ofs << dot.str();
  ofs.close();
  std::cout << dotFilename << " generated succesfully" << std::endl;

  std::string sizeLimit;
  if (extension == "png" || extension == "jpg" || extension == "webp") {
    sizeLimit = " -Gsize=20,20! -Gdpi=200 ";
  }
  std::string command = DOT_PATH + " -T" + extension + sizeLimit + " \"" +
                        dotFilename + "\" -o \"" + filename + "\"";
  std::cout << "Executing: " + command << std::endl;
  if (std::system(command.c_str()) != 0) {
    std::cerr << "DOT command failed. Check the file: " << dotFilename << "\n";
    throw std::runtime_error("dot command failed.");
  }
  std::cout << "Done - cleaning up " << dotFilename << std::endl;
  std::remove(dotFilename.c_str());
}

json DFA::toJson() const {
  json j;
  j["type"] = "DFA";

  // Alphabet
  for (char symbol : alphabet) {
    j["alphabet"].push_back(std::string(1, symbol));
  }

  // States
  for (const std::string &stateName : states) {
    j["states"].push_back(
        {{"name", stateName},
         {"starting", stateName == initialState},
         {"accepting", acceptingStates.count(stateName) > 0}});
  }

  // Transitions
  for (const auto &[fromState, transitions] : transitionTable) {
    for (const auto &[input, toState] : transitions) {
      j["transitions"].push_back({{"from", fromState},
                                  {"to", toState},
                                  {"input", std::string(1, input)}});
    }
  }

  return j;
}
void DFA::saveToFile(const std::string &filename) const {
  json j = toJson();
  std::ofstream out(filename);
  if (!out) {
    throw std::runtime_error("Cannot write to file: " + filename);
  }
  out << std::setw(4) << j << std::endl;
}
void DFA::expandAlphabet(const std::set<char>& fullAlphabet) {
  std::string dummy = "__DEAD__";
  bool usedDummy = false;

  // Voeg dummy-state toe als die nog niet bestaat
  if (states.find(dummy) == states.end()) {
    states.insert(dummy);
    for (char c : fullAlphabet) {
      transitionTable[dummy][c] = dummy; // self-loop
    }
  }

  for (const auto& state : states) {
    for (char c : fullAlphabet) {
      if (transitionTable[state].find(c) == transitionTable[state].end()) {
        transitionTable[state][c] = dummy;
        usedDummy = true;
      }
    }
  }

  if (usedDummy) {
    // Zorg dat dummy geen accepting state is
    acceptingStates.erase(dummy);
  }
}


} // namespace Norkella
