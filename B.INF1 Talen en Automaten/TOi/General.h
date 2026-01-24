#include <algorithm>
#include <iosfwd>
#include <sstream>
#include <unordered_set>
#include <vector>

#ifndef GENERAL_H
#define GENERAL_H
std::string setToString(const std::unordered_set<std::string> &state_set);
std::string makeOneState(const std::string &state1 , const std::string &state2);
std::pair<std::string, std::string> splitState(const std::string &combinedState);
#endif //GENERAL_H
