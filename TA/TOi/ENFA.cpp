#include "ENFA.h"

#include "DFA.h"
#include "json.hpp"
#include "General.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <unordered_map>
#include <unordered_set>
using Json = nlohmann::json;
using namespace std;

ENFA::ENFA(const string& filename) {
    ifstream input(filename);
    if (!input) {
        throw std::logic_error("File: " + filename + " could not be opened.");
    }
    Json json = Json::parse(input);
    if (json["type"] != "ENFA") {
        cerr << "Can only parse ENFA not " << json["type"] << endl;
        return;
    }

    epsilon = json["eps"].get<string>()[0];

    // Read states
    for (const auto& state : json["states"]) {
        string name = state["name"];
        if (state.value("starting", false)) {
            start_state = name;
        }
        if (state.value("accepting", false)) {
            accepting_states.insert(name);
        }
    }

    // Read transitions
    for (const auto& transition : json["transitions"]) {
        const std::string from_state = transition["from"];
        const char input_char = transition["input"].get<std::string>()[0];
        const std::string to_state = transition["to"];

        // Ensure the inner map exists for the 'from' state
        transition_table[from_state][input_char].insert(to_state);
    }
}

bool ENFA::accepts(const string &input) const {
  return this->toDFA().accepts(input);
}

void ENFA::print() const {
  Json j;
  j["type"] = "NFA";
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
    state_obj["starting"] =
        (start_state == state.first);
    state_obj["accepting"] =
        (accepting_states.find(state.first) != accepting_states.end());
    j["states"].push_back(state_obj);
  }

  j["transitions"] = Json::array();
  for (const auto &state : transition_table) {
    for (const auto &transition : state.second) {
      for (const auto &target : transition.second) {
        Json transition_obj;
        transition_obj["from"] = state.first;
        transition_obj["to"] = target;
        transition_obj["input"] = string(1, transition.first);
        j["transitions"].push_back(transition_obj);
      }
    }
  }

  cout << setw(4) << j << endl;
}
const std::unordered_map<
    std::string, std::unordered_map<char, std::unordered_set<std::string>>> &
ENFA::getTransitionTable() const {
  return transition_table;
}

const std::unordered_set<std::string> &ENFA::getAcceptingStates() const {
  return accepting_states;
}

const std::string &ENFA::getStartStates() const {
  return start_state;
}
std::unordered_set<char> ENFA::getAlphabet() const {
    std::unordered_set<char> alphabet;
    for (const auto& state_transitions : transition_table) {
        for (const auto& transition : state_transitions.second) {
            if (transition.first != epsilon) { // Exclude epsilon from the alphabet
                alphabet.insert(transition.first);
            }
        }
    }
    return alphabet;
}

void ENFA::setTransitionTable(
    const std::unordered_map<
        std::string, std::unordered_map<char, std::unordered_set<std::string>>>
        &table) {
  transition_table = table;
}

void ENFA::setAcceptingStates(const std::unordered_set<std::string> &states) {
  accepting_states = states;
}
void ENFA::setStartState(const std::string &state) {
  start_state = state;
}

DFA ENFA::toDFA() const {
    DFA dfa;
    std::queue<std::unordered_set<std::string>> state_queue;
    std::unordered_set<std::string> visited;

    // Begin with the ε-closure of the start state of the eNFA
    std::unordered_set<std::string> start_set = epsilonClosure({getStartStates()});
    std::string start_state_str = setToString(start_set);
    state_queue.push(start_set);
    visited.insert(start_state_str);

    // Loop until there are no more states to process
    while (!state_queue.empty()) {
        std::unordered_set<std::string> current_set = state_queue.front();
        state_queue.pop();

        // Determine if the current set contains any accepting states
        bool is_accepting = false;
        for (const auto& state : current_set) {
            if (getAcceptingStates().find(state) != getAcceptingStates().end()) {
                is_accepting = true;
                break; // Stop if we find an accepting state
            }
        }
        if (is_accepting) {
            dfa.addAcceptingState(setToString(current_set));
        }

        // For each input in the alphabet
        for (char input : getAlphabet()) {
            std::unordered_set<std::string> next_set;
            for (const auto& state : current_set) {
                // Check if the state exists in the eNFA's transition table
                const auto& state_transitions = getTransitionTable().find(state);
                if (state_transitions != getTransitionTable().end()) {
                    // Check if there is a transition for the input character
                    const auto& input_transitions = state_transitions->second.find(input);
                    if (input_transitions != state_transitions->second.end()) {
                        const auto& targets = input_transitions->second;
                        next_set.insert(targets.begin(), targets.end());
                    }
                }
            }

            // Compute the ε-closure of the next_set
            std::unordered_set<std::string> next_set_closure = epsilonClosure(next_set);

            if (!next_set_closure.empty()) {
                std::string next_state_str = setToString(next_set_closure);
                dfa.addTransition(setToString(current_set), input, next_state_str);
                // Add the new state to the queue if it hasn't been visited
                if (visited.find(next_state_str) == visited.end()) {
                    state_queue.push(next_set_closure);
                    visited.insert(next_state_str);
                }
            }
        }
    }

    dfa.setStartState(start_state_str); // Set the start state of the DFA
    return dfa; // Return the constructed DFA
}

std::unordered_set<std::string>
ENFA::epsilonClosure(const std::unordered_set<std::string> &states) const {
  std::unordered_set<std::string> closure = states;
  std::queue<std::string> queue;
  for (const auto &state : states) {
    queue.push(state);
  }

  while (!queue.empty()) {
    std::string current_state = queue.front();
    queue.pop();

    const auto &state_transitions = getTransitionTable().find(current_state);
    if (state_transitions != getTransitionTable().end()) {
      const auto &epsilon_transitions =
          state_transitions->second.find(ENFA::epsilon);
      if (epsilon_transitions != state_transitions->second.end()) {
        for (const auto &target : epsilon_transitions->second) {
          if (closure.find(target) == closure.end()) {
            closure.insert(target);
            queue.push(target);
          }
        }
      }
    }
  }

  return closure;
}

void ENFA::printStats() const {
  // Print the number of states
  std::cout << "no_of_states=" << transition_table.size() << "\n";

  std::map<char, unsigned int> no_of_transitions;
  std::map<unsigned int, unsigned int> degree;

  for (auto &transition : transition_table) {
    // Count the degree (=number of transitions for each state)
    unsigned int num_transitions = transition.second.size();
    degree[num_transitions]++;

    // Count the number of transitions for each symbol
    for (auto &arrow : transition.second) {
      no_of_transitions[arrow.first]++;
    }
  }

  // Print the number of transitions for each symbol
  for (auto &arrow : no_of_transitions) {
    std::cout << "no_of_transitions[" << arrow.first << "]=" << arrow.second
              << "\n";
  }

  // Print the degree distribution
  for (auto &deg : degree) {
    std::cout << "degree[" << deg.first << "]=" << deg.second << "\n";
  }
}
void ENFA::setEpsilon(char eps) {
  epsilon = eps;
}
char ENFA::getEpsilon() const {
  return epsilon;
}

ENFA ENFA::operator+(const ENFA &other) const {
  ENFA result;

  // Create new start state
  std::string new_start_state = "new_start";
  result.setStartState(new_start_state);

  // Create new accepting state
  std::string new_accepting_state = "new_accept";
  result.setAcceptingStates({new_accepting_state});

  // Add epsilon transitions from new start state to the start states of both
  // ENFAs
  result.transition_table[new_start_state][epsilon].insert(this->start_state);
  result.transition_table[new_start_state][epsilon].insert(other.start_state);

  // Add all transitions from the first ENFA
  for (const auto &state_transitions : this->transition_table) {
    for (const auto &input_transitions : state_transitions.second) {
      for (const auto &target : input_transitions.second) {
        result
            .transition_table[state_transitions.first][input_transitions.first]
            .insert(target);
      }
    }
  }

  // Add all transitions from the second ENFA
  for (const auto &state_transitions : other.transition_table) {
    for (const auto &input_transitions : state_transitions.second) {
      for (const auto &target : input_transitions.second) {
        result
            .transition_table[state_transitions.first][input_transitions.first]
            .insert(target);
      }
    }
  }

  // Add epsilon transitions from the accepting states of both ENFAs to the new
  // accepting state
  for (const auto &accepting_state : this->accepting_states) {
    result.transition_table[accepting_state][epsilon].insert(
        new_accepting_state);
  }
  for (const auto &accepting_state : other.accepting_states) {
    result.transition_table[accepting_state][epsilon].insert(
        new_accepting_state);
  }
  return result;
}

ENFA ENFA::operator^(const ENFA &other) const {
  ENFA result;

  // Create a new start state
  std::string new_start_state = "new_start";
  result.setStartState(new_start_state);

  // Create a new accepting state
  std::string new_accepting_state = "new_accept";
  result.setAcceptingStates({new_accepting_state});

  // Add epsilon transitions from the new start state to the start states of both ENFAs
  result.transition_table[new_start_state][epsilon].insert(this->start_state);

  // Copy all transitions from the first ENFA
  for (const auto& state_transitions : this->transition_table) {
    for (const auto& input_transitions : state_transitions.second) {
      for (const auto& target : input_transitions.second) {
        result.transition_table[state_transitions.first][input_transitions.first].insert(target);
      }
    }
  }

  // Add epsilon transitions from the accepting states of the first ENFA to the start state of the second ENFA
  for (const auto& accepting_state : this->accepting_states) {
    result.transition_table[accepting_state][epsilon].insert(other.start_state);
  }

  // Copy all transitions from the second ENFA
  for (const auto& state_transitions : other.transition_table) {
    for (const auto& input_transitions : state_transitions.second) {
      for (const auto& target : input_transitions.second) {
        result.transition_table[state_transitions.first][input_transitions.first].insert(target);
      }
    }
  }

  // Add epsilon transitions from the accepting states of the second ENFA to the new accepting state
  for (const auto& accepting_state : other.accepting_states) {
    result.transition_table[accepting_state][epsilon].insert(new_accepting_state);
  }

  return result;
}

ENFA ENFA::repeat() const {
  ENFA result;

  // Create a new start state
  std::string new_start_state = "new_start";
  result.setStartState(new_start_state);

  // Create a new end state
  std::string new_end_state = "new_end";
  result.setAcceptingStates({new_end_state});

  // Add epsilon transition from the new start state to the old start state
  result.transition_table[new_start_state][epsilon].insert(this->start_state);

  // Add epsilon transition from the new start state to the new end state (to allow for 0 repeats)
  result.transition_table[new_start_state][epsilon].insert(new_end_state);

  // Copy all transitions from the original ENFA
  for (const auto& state_transitions : this->transition_table) {
    for (const auto& input_transitions : state_transitions.second) {
      for (const auto& target : input_transitions.second) {
        result.transition_table[state_transitions.first][input_transitions.first].insert(target);
      }
    }
  }

  // Add epsilon transitions from all old end states to the new end state
  for (const auto& old_end_state : this->accepting_states) {
    result.transition_table[old_end_state][epsilon].insert(new_end_state);
  }

  // Add epsilon transitions from all old end states to the old start state (to allow for repetition)
  for (const auto& old_end_state : this->accepting_states) {
    result.transition_table[old_end_state][epsilon].insert(this->start_state);
  }

  return result;
}