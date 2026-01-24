#ifndef DFA_H
#define DFA_H

#include "../json.hpp"
#include "RE.h"
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_set>
namespace Norkella {
using json = nlohmann::json;

class DFA {
  std::set<std::string> states;
  std::set<char> alphabet;
  std::map<std::string, std::map<char, std::string>> transitionTable;
  std::string initialState;
  std::unordered_set<std::string> acceptingStates;
  void loadFromJSON(const json &j);

public:
  DFA() = default;
  explicit DFA(const std::string &filename);
  // constructor voor product
  DFA(const DFA &dfa1, const DFA &dfa2, bool intersection);
  bool accepts(const std::string &input);
  void print();
  void setAlphabet(const std::set<char> &alphabet);
  void addState(const std::string &name, bool isStarting, bool isAccepting);
  void addTransition(const std::string &from, char input,
                     const std::string &to);
  std::string getTransition(const std::string &from, char input) const;
  const std::set<char> &getAlphabet() const;
  const std::string &getInitialState() const;
  bool isAcceptingState(const std::string &state) const;
  const std::map<char, std::string> &
  getTransitions(const std::string &state) const;
  DFA minimize(const bool smallStateNames = true) const;
  void smallerStateNames();
  void printTable() const;
  friend bool operator==(const DFA &dfa1, const DFA &dfa2);
  const std::set<std::string> &getStates() const;
  RE toRE() const;
  json toJson() const;
  void saveToFile(const std::string& filename) const;
  void toImage(const std::string &filename, const std::string &extension);
  void expandAlphabet(const std::set<char>& fullAlphabet) ;
};
} // namespace Norkella

#endif // DFA_H
