#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

class DFA {
private:
    unordered_map<string, unordered_map<char, string>> transition_table;
    unordered_set<string> accepting_states;
    string start_state;

public:
    // Default constructor - hardcoded DFA
    explicit DFA(const string& filename);

    // Check if the input string is accepted by the DFA
    bool accepts(const string& input);
    void print() const;
};
