#ifndef ENFA_H
#define ENFA_H

#include "DFA.h"
#include <algorithm>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>
namespace Adriaan {
class ENFA;
}
namespace Norkella {
class ENFA {
private:
    std::set<char> alphabet;
    char epsilon;
    std::set<std::string> states;
    std::string initialState;
    std::set<std::string> acceptingStates;
    std::map<std::string, std::map<char, std::set<std::string>>> transitionTable;

    std::set<std::string> epsilonClosure(const std::set<std::string>& states);
    std::set<std::string> move(const std::set<std::string>& states, char symbol);
    std::string getStateName(const std::set<std::string>& states);
    bool acceptsHelper(const std::set<std::string>& currentStates, const std::string& remainingInput);

public:
    explicit ENFA(const std::string& filename);
    ENFA() = default;
    ENFA(const Adriaan::ENFA & enfa);
    DFA toDFA(bool useSmallNames = true);
    void kleeneStar();
    void concatenate(const ENFA& other);
    void unionWith(const ENFA& other);
    void setEpsilon(char eps);
    void printStats() const;
    bool accepts(const std::string& input);
    void addState(const std::string& name, bool isStarting, bool isAccepting);
    void addTransition(const std::string& from, char input, const std::string& to);
    void setAlphabet(const std::set<char>& alphabet);
    bool isAcceptingState(const std::string& state) const;
};
}

#endif // ENFA_H
