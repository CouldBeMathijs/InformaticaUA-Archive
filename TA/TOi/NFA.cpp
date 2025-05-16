#include "NFA.h"

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

NFA::NFA(const string& filename) {
    ifstream input(filename);
    if (!input) {
        throw std::logic_error("File: " + filename + " could not be opened.");
    }
    Json json = Json::parse(input);
    if (json["type"] != "NFA") {
        cerr << "Can only parse NFA not " << json["type"] << endl;
        return;
    }

    for (const auto& state : json["states"]) {
        string name = state["name"];
        if (state.value("starting", false)) {
            start_states.insert(name);
        }
        if (state.value("accepting", false)) {
            accepting_states.insert(name);
        }
    }

    for (const auto& transition : json["transitions"]) {
        const std::string from_state = transition["from"];
        const char input_char = transition["input"].get<std::string>()[0];
        const std::string to_state = transition["to"];

        // Ensure the inner map exists for the 'from' state
        transition_table[from_state][input_char].insert(to_state);
    }
}

bool NFA::accepts(const string& input) const {
  return this->toDFA().accepts(input);
}

void NFA::print() const {
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
        (start_states.find(state.first) != start_states.end());
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
NFA::getTransitionTable() const {
  return transition_table;
}

const std::unordered_set<std::string> &NFA::getAcceptingStates() const {
  return accepting_states;
}

const std::unordered_set<std::string> &NFA::getStartStates() const {
  return start_states;
}
std::unordered_set<char> NFA::getAlphabet() const {
  std::unordered_set<char> alphabet;
  for (const auto &state_transitions : transition_table) {
    for (const auto &transition : state_transitions.second) {
      alphabet.insert(transition.first);
    }
  }
  return alphabet;
}


void NFA::setTransitionTable(
    const std::unordered_map<
        std::string, std::unordered_map<char, std::unordered_set<std::string>>>
        &table) {
  transition_table = table;
}

void NFA::setAcceptingStates(const std::unordered_set<std::string> &states) {
  accepting_states = states;
}

void NFA::setStartStates(const std::unordered_set<std::string> &states) {
  start_states = states;
}

DFA NFA::toDFA() const {
        DFA dfa; // Create a DFA object
        queue<std::unordered_set<std::string>> state_queue;
        std::unordered_set<std::string> visited;

        // Begin with the start state of the NFA
        const std::unordered_set<std::string>& start_set = getStartStates();
        std::string start_state_str = setToString(start_set);
        state_queue.push(start_set);
        visited.insert(start_state_str);

        // Loop until there are no more states to process
        while (!state_queue.empty()) {
            std::unordered_set<std::string> current_set = state_queue.front();
            state_queue.pop();

            // Determine if the current set contains any accepting states
            for (const auto& state : current_set) {
                if (getAcceptingStates().find(state) != getAcceptingStates().end()) {
                    dfa.addAcceptingState(setToString(current_set));
                    break; // Stop if we find an accepting state
                }
            }

            // For each input in the alphabet
            for (char input : getAlphabet()) {
                std::unordered_set<std::string> next_set;
                for (const auto& state : current_set) {
                    // Check if the state exists in the NFA's transition table
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

                if (!next_set.empty()) {
                    std::string next_state_str = setToString(next_set);
                    dfa.addTransition(setToString(current_set), input, next_state_str);
                    // Add the new state to the queue if it hasn't been visited
                    if (visited.find(next_state_str) == visited.end()) {
                        state_queue.push(next_set);
                        visited.insert(next_state_str);
                    }
                }
            }
        }

        dfa.setStartState(start_state_str); // Set the start state of the DFA
        return dfa; // Return the constructed DFA
    }