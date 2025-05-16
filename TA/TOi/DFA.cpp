#include "DFA.h"

#include "General.h"
#include "json.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <set>
#include <utility>
using Json = nlohmann::json;

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>

using namespace std;

void printTableStatic(const unordered_map<string, unordered_map<string, int>>& table, bool printNumbers) {
  // Extract and sort the keys (labels)
  vector<string> labels;
  for (const auto& row : table) {
    labels.push_back(row.first);
  }
  sort(labels.begin(), labels.end());

  int n = labels.size();

  // Print the table body
  for (int i = 1; i < n; ++i) {
    cout << labels[i] << "\t"; // Row label

    for (int j = 0; j < i; ++j) {
      string rowLabel = labels[i];
      string colLabel = labels[j];

      // Get the value if exists
      auto rowIt = table.find(rowLabel);
      if (rowIt != table.end()) {
        auto colIt = rowIt->second.find(colLabel);
        if (colIt != rowIt->second.end()) {
          int val = colIt->second;
          if (val == -1) {
            cout << "-\t";
          } else if (val >= 0 && val <= 9) {
            cout << (printNumbers ? to_string(val) : "X") << "\t";
          } else if (val > 9) {
            cout << "X\t";
          } else {
            cout << "?\t";
          }
          continue;
        }
      }

      // If value doesn't exist
      cout << "?\t";
    }

    cout << "\n";
  }

  // Print the header row
  cout << "\t";
  for (int j = 0; j < n - 1; ++j) {
    cout << labels[j] << "\t";
  }
  cout << "\n";
}

DFA::DFA(const string &filename) {
  ifstream input(filename);
  Json json = Json::parse(input);
  if (json["type"] != "DFA") {
    cerr << "Can only parse DFA not " << json["type"] << endl;
    return;
  }

  for (const auto &state : json["states"]) {
    string name = state["name"];
    if (state.value("starting", false)) {
      start_state = name;
    }
    if (state.value("accepting", false)) {
      accepting_states.insert(name);
    }
  }

  for (const auto &transition : json["transitions"]) {
    transition_table[transition["from"]][transition["input"].get<string>()[0]] =
        transition["to"];
  }
}

bool DFA::sameAlphabet(const DFA& other) const{
  auto myalphabet = getAlphabet();
  auto otheralphabet = other.getAlphabet();
  if (myalphabet.size() != otheralphabet.size()) {
    return false;
  }
  for (char ch : myalphabet) {
    if (otheralphabet.find(ch) == otheralphabet.end()) {
      return false;
    }
  }
  return true;
}

DFA::DFA(const DFA& dfa1, const DFA& dfa2, bool isIntersection) {
  if (!dfa1.sameAlphabet(dfa2)) {
    throw std::invalid_argument("The 2 DFA's should have the same alphabet");
  }

  start_state = makeOneState(dfa1.getStartState(), dfa2.getStartState());
  std::queue<std::string> stateQueue;
  stateQueue.push(start_state);

  std::unordered_set<std::string> visited;

  while (!stateQueue.empty()) {
    std::string currentState = stateQueue.front();
    stateQueue.pop();

    if (visited.find(currentState) != visited.end()) continue;
    visited.insert(currentState);

    // Extract original DFA states
    auto [name1, name2] = splitState(currentState); // Function to extract original states

    // Ensure both states exist
    if (dfa1.getTransitionTable().count(name1) == 0 || dfa2.getTransitionTable().count(name2) == 0) {
      continue;
    }

    std::unordered_map<char, std::string> transition_set;

    for (const auto& [symbol, next1] : dfa1.getTransitionTable().at(name1)) {
      if (dfa2.getTransitionTable().at(name2).count(symbol) == 0) continue;

      std::string next2 = dfa2.getTransitionTable().at(name2).at(symbol);
      std::string newState = makeOneState(next1, next2);

      transition_set[symbol] = newState;

      if (visited.find(newState) == visited.end()) {
        stateQueue.push(newState);
      }
    }

    transition_table[currentState] = transition_set;

    bool inDfa1 = dfa1.getAcceptingStates().count(name1);
    bool inDfa2 = dfa2.getAcceptingStates().count(name2);

    if ((isIntersection && inDfa1 && inDfa2) || (!isIntersection && (inDfa1 || inDfa2))) {
      accepting_states.insert(currentState);
    }
  }
}


DFA::DFA() = default;

bool DFA::accepts(const string& input) {
    string current_state = start_state;

    for (char c : input) {
        if (transition_table[current_state].find(c) == transition_table[current_state].end()) {
            return false; // Invalid transition
        }
        current_state = transition_table[current_state][c];
    }

    return accepting_states.find(current_state) != accepting_states.end();
}

void DFA::print() const {
  Json j;
  j["type"] = "DFA";
  j["alphabet"] = Json::array();
  unordered_set<char> alphabet;
  for (const auto &state : transition_table) {
    for (const auto &transition : state.second) {
      alphabet.insert(transition.first);
    }
  }
  for (char c : alphabet) {
    j["alphabet"].push_back(string(1, c));
  }

  j["states"] = Json::array();
  for (const auto &state : transition_table) {
    Json state_obj;
    state_obj["name"] = state.first;
    state_obj["starting"] = (state.first == start_state);
    state_obj["accepting"] =
        (accepting_states.find(state.first) != accepting_states.end());
    j["states"].push_back(state_obj);
  }

  j["transitions"] = Json::array();
  for (const auto &state : transition_table) {
    for (const auto &transition : state.second) {
      Json transition_obj;
      transition_obj["from"] = state.first;
      transition_obj["to"] = transition.second;
      transition_obj["input"] = string(1, transition.first);
      j["transitions"].push_back(transition_obj);
    }
  }

  cout << setw(4) << j << endl;
}
const unordered_map<string, unordered_map<char, string>> & DFA::getTransitionTable() const {
  return transition_table;
}

const unordered_set<string> &DFA::getAcceptingStates() const {
  return accepting_states;
}

const string &DFA::getStartState() const {
  return start_state;
}

void DFA::setTransitionTable(
    const unordered_map<string, unordered_map<char, string>>
        &new_transition_table) {
  transition_table = new_transition_table;
}

void DFA::setAcceptingStates(
    const unordered_set<string> &new_accepting_states) {
  accepting_states = new_accepting_states;
}

void DFA::setStartState(const string &new_start_state) {
  start_state = new_start_state;
}
void DFA::addAcceptingState(const std::string &state) {
  accepting_states.insert(state);
}
void DFA::addTransition(const std::string &from_state, char input,
                        const std::string &to_state) {
  transition_table[from_state][input] = to_state;
}

unordered_map<string, unordered_map<string, int>> DFA::doTFA() const {
  //cout << "\n==Setting up a clean tabel\n" << endl;
  unordered_map<string, unordered_map<string, int>> table;
  auto states = getStates();
  unordered_set<string> non_accepting_states;
  //cout << "Creating place in table for:" << endl;
  for (auto i : states) {
    // Create the table to be empty
    for (auto j : states) {
      table[i][j] = 0;
      //cout << i << ", " << j << endl;
    }
    // Maybe add this state to the non_accepting_states for later use
    if (accepting_states.find(i) == accepting_states.end()) {
      non_accepting_states.insert(i);
    }
  }
  //cout <<"\n\nNow printing empty table:\n";
  // Debug
  //printTableStatic(table, true);

  //cout << "\n\nStarting with generation X0" << endl;

  // Do x_0
  for (const auto& accepting_state1: accepting_states) {
    for (const auto& accepting_state2 : accepting_states) {
      table[accepting_state1][accepting_state2] = -1;
      table[accepting_state2][accepting_state1] = -1;
      //printTableStatic(table, true);
    }
  }
  for (const auto& non_accepting_state1: non_accepting_states) {
    for (const auto& non_accepting_state2 : non_accepting_states) {
      table[non_accepting_state1][non_accepting_state2] = -1;
      table[non_accepting_state2][non_accepting_state1] = -1;
      //printTableStatic(table, true);
    }
  }
  auto alphabet = getAlphabet();


  //cout << "Creating the reverse transition table so it can be looked at more easily in the future:" << endl;
  unordered_map<char, unordered_map<string, unordered_set<string>>> reverse_transitions;
  for (auto ch: alphabet) {
    for (const auto& state : states) {
      reverse_transitions[ch][transition_table.at(state).at(ch)].insert(state);
    }
  }
  int currentLoop = 1;
  bool changed = true;
  while (changed) {
    //cout << "Something changed, now on loop " << currentLoop << endl;
    changed = false;
    for (const auto& state1: states) {
      for (const auto& state2 : states) {
        if (state1 == state2) {
          //cout << "Skipping " << state1 << ", " << state2 << " as we don't check doubles" << endl;
          continue;
        }
        // Als de staat nog niet aangekruist is, skip hem
        if (table[state1][state2] != currentLoop - 1 || table[state2][state1] != currentLoop - 1) {
          if (!(table[state1][state2] != currentLoop - 1 && table[state2][state1] != currentLoop - 1)) {
            throw std::logic_error(state1 + ", " + state2 + " did not match: loop " + to_string(currentLoop));
          }
          if (table[state1][state2] == -1) {
            //cout << "Skipping state combo " << state1 <<  ", " << state2 << ", as it was an empty cell" << endl;
          } else {
            //cout << "Skipping state combo " << state1 << ", " << state2 << ", as it is for loop " << table[state1][state2] << " currently checking for states in loop " << currentLoop - 1 << endl;
          }
          continue;
        }
        //cout << "Found crossed out cell '" << state1 << ", " << state2 << "' - Searching for new cells to cross out" << endl;
        for (auto ch : alphabet) {
          if (reverse_transitions[ch].find(state1) == reverse_transitions[ch].end()) {
            //cout << "Did not find first state '" << state1 << "' in the column for '" << ch << "' skipping adding reverse transitions" << endl;
            continue;
          }
          if (reverse_transitions[ch].find(state2) == reverse_transitions[ch].end()) {
            //cout << "Did not find second state '" << state2 << "' in the column for '" << ch << "' skipping adding reverse transitions" << endl;
            continue;
          }
          auto &set1 = reverse_transitions[ch].at(state1);
          auto &set2 = reverse_transitions[ch].at(state2);
          //cout << "Found matching reverse transitions for '" << state1 << ", " << state2 << "' over alphabet '" << ch << "'" << endl;
          //cout << "\tMaking ";
          for (const auto& rev1 : set1) {
            for (const auto& rev2 : set2) {
              if (table[rev1][rev2] != -1) {
                //cout << "'(will not change '" << rev1 << ", " << rev2 << "' and '"<< rev2 << ", " << rev1 << "', because not empty); ";
                continue;
              }
              table[rev1][rev2] = currentLoop;
              //cout << "'" << rev1 << ", " << rev2 << "' : '" << rev2 << ", " << rev1 << "; ";
              table[rev2][rev1] = currentLoop;
              changed = true;
            }
          }
          //cout << currentLoop << endl;
        }
        //printTableStatic(table, true);
      }
    }
    currentLoop++;
  }
  return table;
}

DFA DFA::mergeDFA(const DFA &other) const {
  // The alphabets must be equal
  if (!sameAlphabet(other)) {
    cerr << "Debug :" << endl;
    cerr << "Alphabet 1: ";
    for (char ch : getAlphabet()) {
      cerr << ch << " ";
    }
    cerr << "\nAlphabet 2: ";
    for (char ch : other.getAlphabet()) {
      cerr << ch << " ";
    }
    cerr << endl;

    throw std::invalid_argument("The alphabets are not equal");
  }
  // The state names must be unique
  auto mystates = getStates();
  auto otherstates = other.getStates();
  std::unordered_set<std::string> myset(mystates.begin(), mystates.end());
  std::unordered_set<std::string> otherset(otherstates.begin(), otherstates.end());

  for (std::unordered_set<std::string>::const_iterator it = myset.begin(); it != myset.end(); ++it) {
    if (otherset.find(*it) != otherset.end()) {
      throw std::invalid_argument("State sets must be disjoint (no overlap)");
    }
  }

  DFA mergedDFA;
  mergedDFA.setStartState("Invalid DFA");
  // Merge accepting states
  for (const auto &accepting_state : accepting_states) {
    mergedDFA.addAcceptingState(accepting_state);
  }
  for (const auto &accepting_state : other.getAcceptingStates()) {
    mergedDFA.addAcceptingState(accepting_state);
  }
  // Merge transition tables
  std::unordered_map<std::string, std::unordered_map<char, std::string>>
      other_transition_table = other.getTransitionTable();
  std::unordered_map<std::string, std::unordered_map<char, std::string>>
      out_transition_table = transition_table;

  for (std::unordered_map<std::string,
                          std::unordered_map<char, std::string>>::const_iterator
           outer_it = other_transition_table.begin();
       outer_it != other_transition_table.end(); ++outer_it) {

    const std::string &state = outer_it->first;
    const std::unordered_map<char, std::string> &inner_map = outer_it->second;

    // If state already exists in out_transition_table, merge inner maps
    if (out_transition_table.find(state) != out_transition_table.end()) {
      std::unordered_map<char, std::string> &existing_inner =
          out_transition_table[state];
      for (std::unordered_map<char, std::string>::const_iterator inner_it =
               inner_map.begin();
           inner_it != inner_map.end(); ++inner_it) {
        existing_inner[inner_it->first] =
            inner_it->second; // overwrite if exists
      }
    } else {
      // New state, just insert the whole inner map
      out_transition_table[state] = inner_map;
    }
  }
  mergedDFA.setTransitionTable(out_transition_table);
  return mergedDFA;
}

bool DFA::operator==(const DFA& other) const {
  DFA mergedDFA = mergeDFA(other);
  auto table = mergedDFA.doTFA();
  printTableStatic(table, false);
  return table.at(start_state).at(other.getStartState()) == -1;
}

DFA DFA::minimize() const {
  vector<pair<string, string>> rawPairs;
  unordered_map<string, unordered_map<string, int>> table = doTFA();

  // Step 1: Collect all equivalent pairs (val == -1)
  for (auto& [state1, transitions] : table) {
    for (auto& [state2, val] : transitions) {
      if (val == -1) {
        rawPairs.emplace_back(state1, state2);
      }
    }
  }

  // Step 2: Group equivalent states
  vector<unordered_set<string>> equivalents;
  for (auto& [a, b] : rawPairs) {
    bool found = false;
    for (auto& group : equivalents) {
      if (group.count(a) || group.count(b)) {
        group.insert(a);
        group.insert(b);
        found = true;
        break;
      }
    }
    if (!found) {
      equivalents.push_back({a, b});
    }
  }

  // Step 3: Merge overlapping equivalence groups
  bool merged;
  do {
    merged = false;
    for (size_t i = 0; i < equivalents.size(); ++i) {
      for (size_t j = i + 1; j < equivalents.size(); ++j) {
        unordered_set<string> intersection;
        for (const auto& s : equivalents[i]) {
          if (equivalents[j].count(s)) {
            intersection.insert(s);
          }
        }
        if (!intersection.empty()) {
          equivalents[i].insert(equivalents[j].begin(), equivalents[j].end());
          equivalents.erase(equivalents.begin() + j);
          merged = true;
          break;
        }
      }
      if (merged) break;
    }
  } while (merged);

  // Step 4: Map each state to its new name
  unordered_map<string, string> stateToNewName;
  for (const auto& group : equivalents) {
    string newname;
    if (group.size() == 1) {
      newname = "{" + *group.begin() + "}";
    } else {
      std::vector<std::string> sorted_group(group.begin(), group.end());
      std::sort(sorted_group.begin(), sorted_group.end());

      newname = "{";
      bool first = true;
      for (const std::string& state : sorted_group) {
        if (!first) newname += ", ";
        newname += state;
        first = false;
      }
      newname += "}";
    }

    for (const auto& state : group) {
      stateToNewName[state] = newname;
    }
  }

  // Step 5: Handle states not in any equivalence group
  for (const auto& [state, _] : transition_table) {
    if (stateToNewName.find(state) == stateToNewName.end()) {
      stateToNewName[state] = state;
    }
  }

  // Step 6: Build minimized DFA
  DFA out;
  unordered_map<string, unordered_map<char, string>> newTable;
  auto alphabet = getAlphabet();

  for (const auto& [oldState, transitions] : transition_table) {
    string from = stateToNewName[oldState];
    for (char ch : alphabet) {
      string to = transitions.at(ch);
      string newTo = stateToNewName[to];
      newTable[from][ch] = newTo;
    }
  }

  out.setTransitionTable(newTable);

  // Set new start state
  out.setStartState(stateToNewName.at(start_state));

  // Set new accepting states
  for (const auto& acc : accepting_states) {
    out.addAcceptingState(stateToNewName.at(acc));
  }

  return out;
}

void DFA::printTable() const {
  printTableStatic(doTFA(), false);
}

std::unordered_set<char> DFA::getAlphabet() const {
  std::unordered_set<char> alphabet; // To store unique input symbols
  // Iterate through the transition table
  for (const auto& state_pair : transition_table) {
    const auto& transitions = state_pair.second; // Get the inner map (transitions for the state)
    for (const auto& transition_pair : transitions) {
      char input_symbol = transition_pair.first; // The input symbol
      alphabet.insert(input_symbol); // Insert the symbol into the set
    }
  }

  return alphabet; // Return the set of unique input symbols
}
void DFA::printTransitionTable() const {
  // Get the alphabet and sort it
  std::unordered_set<char> alphabetSet = getAlphabet();
  std::vector<char> alphabet(alphabetSet.begin(), alphabetSet.end());
  std::sort(alphabet.begin(), alphabet.end());
  // Sort states alphabetically
  std::vector<std::string> sortedStates;
  for (const auto &state_pair : transition_table) {
    sortedStates.push_back(state_pair.first);
  }
  std::sort(sortedStates.begin(), sortedStates.end());
  // Print header
  std::cout << "\t"; // Initial tab for the header
  for (char symbol : alphabet) {
    std::cout << symbol << "\t"; // Print each symbol in the alphabet
  }
  std::cout << std::endl;

  // Print each state and its transitions
  for (const std::string &state : sortedStates) {
    std::cout << state; // Print the state
    if (state == start_state) {
      cout << "<";
    } if (accepting_states.find(state) != accepting_states.end()) {
      cout << "*";
    }
    cout << "\t";
    for (char symbol : alphabet) {
      // Check if there is a transition for this state and symbol
      if (transition_table.at(state).find(symbol) !=
          transition_table.at(state).end()) {
        std::cout << transition_table.at(state).at(symbol)
                  << "\t"; // Print the next state
      } else {
        std::cout << "-\t"; // Print '-' if no transition exists
      }
    }
    std::cout << std::endl; // New line after each state
  }
}

vector<string> DFA::getStates() const {
  unordered_set<string> all_states;

  // Collect all states from transition table
  for (const auto &[state, transitions] : transition_table) {
    all_states.insert(state);
    for (const auto &[symbol, dest_state] : transitions) {
      all_states.insert(dest_state);
    }
  }

  // Convert to vector and sort alphabetically
  std::vector<string> sorted_states(all_states.begin(), all_states.end());
  std::sort(sorted_states.begin(), sorted_states.end());

  return sorted_states;
}


