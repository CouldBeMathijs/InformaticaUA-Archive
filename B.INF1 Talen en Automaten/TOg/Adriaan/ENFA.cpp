#include "ENFA.h"
#include <queue>
namespace Adriaan {
ENFA::ENFA(char epsilon) : epsilonChar(epsilon), startState(-1) {}

int ENFA::newState() {
  int id = states.empty() ? 0 : *states.rbegin() + 1;
  states.insert(id);
  return id;
}

void ENFA::addTransition(int from, char symbol, int to) {
  transitions[{from, symbol}].insert(to);
  states.insert(from);
  states.insert(to);
}

void ENFA::setStartState(int state) {
  startState = state;
  states.insert(state);
}

void ENFA::addAcceptingState(int state) {
  acceptingStates.insert(state);
  states.insert(state);
}

void ENFA::setAlphabet(const std::set<char> &alpha) { alphabet = alpha; }

void ENFA::printStats() const {
  std::map<char, int> transCount;

  for (const auto &[key, targets] : transitions) {
    int from = key.first;
    char symbol = key.second;
    int targetCount = targets.size();

    transCount[symbol] += targetCount;
  }

  std::map<int, int> stateDegree;
  for (const auto &[key, targets] : transitions) {
    int from = key.first;
    stateDegree[from] += targets.size();
  }

  std::cout << "no_of_states=" << states.size() << "\n";

  std::cout << "no_of_transitions[" << epsilonChar
            << "]=" << transCount[epsilonChar] << "\n";
  for (char c : alphabet)
    std::cout << "no_of_transitions[" << c << "]=" << transCount[c] << "\n";

  std::map<int, int> degreeDist;
  for (int st : states) {
    int deg = stateDegree[st];
    degreeDist[deg]++;
  }

  for (auto [deg, count] : degreeDist) {
    std::cout << "degree[" << deg << "]=" << count << "\n";
  }
}
} // namespace Adriaan