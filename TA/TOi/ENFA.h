#ifndef ENFA_H
#define ENFA_H

#include "DFA.h"

#include <string>
#include <unordered_map>
#include <unordered_set>

class ENFA {
private:
  std::unordered_map<std::string, std::unordered_map<char, std::unordered_set<std::string>>> transition_table;
  std::unordered_set<std::string> accepting_states;
  std::string start_state;
  char epsilon;
public:
  explicit ENFA(const std::string &filename);
  ENFA() = default;

  // Check if the input string is accepted by the NFA
  bool accepts(const std::string &input) const;
  void print() const;

  // Getters
  const std::unordered_map<
      std::string, std::unordered_map<char, std::unordered_set<std::string>>> &
  getTransitionTable() const;

  const std::unordered_set<std::string> &getAcceptingStates() const;

  const std::string &getStartStates() const;

  std::unordered_set<char> getAlphabet() const;

  // Setters
  void setTransitionTable(
      const std::unordered_map<
          std::string,
          std::unordered_map<char, std::unordered_set<std::string>>> &table);

  void setAcceptingStates(const std::unordered_set<std::string> &states);

  void setStartState(const std::string &state);

  DFA toDFA() const;
  std::unordered_set<std::string>
  epsilonClosure(const std::unordered_set<std::string> &states) const;
  void printStats() const;

  void setEpsilon(char eps);

  char getEpsilon() const;

  ENFA operator+(const ENFA &other) const;
  ENFA operator^(const ENFA &other) const;
  ENFA repeat() const;
  void printTransitionTable() const;
};

#endif // ENFA_H
