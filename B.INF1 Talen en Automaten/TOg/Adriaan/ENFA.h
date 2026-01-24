#ifndef ENFA_ADRIAAN_H
#define ENFA_ADRIAAN_H

#include <map>
#include <set>
#include <string>
#include <vector>

#include <iostream>

namespace Adriaan {

class ENFA {
public:
  std::set<char> alphabet;
  std::set<int> states;
  std::map<std::pair<int, char>, std::set<int>> transitions;
  int startState = 0;
  std::set<int> acceptingStates;

  char epsilonChar;

public:
  explicit ENFA(char epsilon);

  int newState();
  void addTransition(int from, char symbol, int to);
  void setStartState(int state);
  void addAcceptingState(int state);
  void setAlphabet(const std::set<char> &alpha);

  void printStats() const;
};
} // namespace Adriaan

#endif // ENFA_ADRIAAN_H