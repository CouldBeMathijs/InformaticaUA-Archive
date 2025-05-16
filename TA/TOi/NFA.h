#ifndef NFA_H
#define NFA_H

#include "DFA.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

class NFA {
private:
  std::unordered_map<std::string, std::unordered_map<char, std::unordered_set<std::string>>> transition_table;
  std::unordered_set<std::string> accepting_states;
  std::unordered_set<std::string> start_states;

public:
  explicit NFA(const std::string &filename);

  // Check if the input string is accepted by the NFA
  bool accepts(const std::string &input) const;
  void print() const;

  // Getters
  const std::unordered_map<
      std::string, std::unordered_map<char, std::unordered_set<std::string>>> &
  getTransitionTable() const;

  const std::unordered_set<std::string> &getAcceptingStates() const;

  const std::unordered_set<std::string> &getStartStates() const;

  std::unordered_set<char> getAlphabet() const;

  // Setters
  void setTransitionTable(
      const std::unordered_map<
          std::string,
          std::unordered_map<char, std::unordered_set<std::string>>> &table);

  void setAcceptingStates(const std::unordered_set<std::string> &states);

  void setStartStates(const std::unordered_set<std::string> &states);

  //static std::string setToString(const std::unordered_set<std::string> &state_set);

  DFA toDFA() const;
  void printTransitionTable() const;
};

#endif // NFA_H
