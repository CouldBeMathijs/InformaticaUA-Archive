#ifndef TOKENMATCHER_H
#define TOKENMATCHER_H

#include <string>
#include <map>
#include <vector>
#include <set>
#include "Norkella/DFA.h"


class TokenMatcher {
public:
    void loadLanguageDFAs(const std::string& folder);
    void analyzeText(const std::string& inputText);
    void printReport() const;
    std::map<std::string, double> getScores() const;

private:
    std::map<std::string, Norkella::DFA> dfaMap;
    std::map<std::string, int> tokenMatches;
    int totalMatchedTokens = 0;
    int totalTokens = 0;
    bool ignoreCase = true;

    std::vector<std::string> tokenize(const std::string& line) const;
};



#endif //TOKENMATCHER_H
