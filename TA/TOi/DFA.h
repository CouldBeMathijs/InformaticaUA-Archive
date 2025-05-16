#ifndef DFA_H
#define DFA_H
#include <map>
#include <sstream>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>
using namespace std;

class DFA {
private:
  unordered_map<string, unordered_map<char, string>> transition_table;
  unordered_set<string> accepting_states;
  string start_state;

public:
  explicit DFA(const string &filename);
  bool sameAlphabet(const DFA &other) const;
  DFA(const DFA &dfa1, const DFA &dfa2, bool isIntersection);

  DFA();

  // Check if the input string is accepted by the DFA
  bool accepts(const string& input);
  void print() const;

  // Getters
  const unordered_map<string, unordered_map<char, string>> &
  getTransitionTable() const;

  const unordered_set<string> &getAcceptingStates() const;

  const string &getStartState() const;

  // Setters
  void
  setTransitionTable(const unordered_map<string, unordered_map<char, string>>
                         &new_transition_table);

  void setAcceptingStates(const unordered_set<string> &new_accepting_states);

  void setStartState(const string &new_start_state);

  void addAcceptingState(const std::string &state);

  void addTransition(const std::string &from_state, char input,
                     const std::string &to_state);
  unordered_map<string, unordered_map<string, int>> doTFA() const;
  DFA mergeDFA(const DFA &other) const;
  bool alphabetsEqual(const unordered_set<char> &set1,
                      const unordered_set<char> &set2);
  bool compareAlphabets(const unordered_set<char> &a,
                        const unordered_set<char> &b);
  bool operator==(const DFA &other) const;
  DFA minimize() const;
  std::unordered_set<char> getAlphabet() const;

  void printTransitionTable() const;

  vector<string> getStates() const;

  void printTable() const;
};
#endif //DFA_H
