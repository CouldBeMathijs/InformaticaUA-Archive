#include "General.h"
std::string setToString(const std::unordered_set<std::string> &state_set) {
  std::vector<std::string> states(state_set.begin(), state_set.end());
  sort(states.begin(),
       states.end()); // Zorg ervoor dat de volgorde alfabetisch is
  std::stringstream ss;
  ss << "{";
  for (size_t i = 0; i < states.size(); ++i) {
    ss << states[i];
    if (i < states.size() - 1) {
      ss << ",";
    }
  }
  ss << "}";
  return ss.str();
}
std::string makeOneState(const std::string &state1, const std::string &state2) {
  return "(" + state1 + "," + state2 + ")";
}
std::pair<std::string, std::string> splitState(const std::string &combinedState) {
  if (combinedState.front() != '(' || combinedState.back() != ')') {
    throw std::invalid_argument("Invalid state format");
  }

  std::string content = combinedState.substr(1, combinedState.size() - 2); // Remove parentheses
  size_t commaPos = content.find(',');

  if (commaPos == std::string::npos) {
    throw std::invalid_argument("Invalid state format, missing comma separator");
  }

  std::string state1 = content.substr(0, commaPos);
  std::string state2 = content.substr(commaPos + 1);

  return {state1, state2};
}
