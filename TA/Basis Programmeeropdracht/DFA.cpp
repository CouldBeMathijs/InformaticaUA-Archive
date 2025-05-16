//
// Created by mathijs on 26/02/25.
//

#include "DFA.h"
#include "json.hpp"
#include <fstream>
#include <iomanip>
using Json = nlohmann::json;
DFA::DFA(const string& filename) {
    ifstream input(filename);
    Json json = Json::parse(input);
    if (json["type"] != "DFA") {
        cerr << "Can only parse DFA not " << json["type"] << endl;
        return;
    }

    for (const auto& state : json["states"]) {
        string name = state["name"];
        if (state.value("starting", false)) {
            start_state = name;
        }
        if (state.value("accepting", false)) {
            accepting_states.insert(name);
        }
    }

    for (const auto& transition : json["transitions"]) {
        transition_table[transition["from"]][transition["input"].get<string>()[0]] = transition["to"];
    }
}

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
    for (const auto& state : transition_table) {
        for (const auto& transition : state.second) {
            alphabet.insert(transition.first);
        }
    }
    for (char c : alphabet) {
        j["alphabet"].push_back(string(1, c));
    }

    j["states"] = Json::array();
    for (const auto& state : transition_table) {
        Json state_obj;
        state_obj["name"] = state.first;
        state_obj["starting"] = (state.first == start_state);
        state_obj["accepting"] = (accepting_states.find(state.first) != accepting_states.end());
        j["states"].push_back(state_obj);
    }

    j["transitions"] = Json::array();
    for (const auto& state : transition_table) {
        for (const auto& transition : state.second) {
            Json transition_obj;
            transition_obj["from"] = state.first;
            transition_obj["to"] = transition.second;
            transition_obj["input"] = string(1, transition.first);
            j["transitions"].push_back(transition_obj);
        }
    }

    cout << setw(4) << j << endl;
}
