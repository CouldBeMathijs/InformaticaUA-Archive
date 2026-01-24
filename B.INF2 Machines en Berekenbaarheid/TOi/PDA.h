#ifndef PDA_H
#define PDA_H
#include <set>
#include <string>
#include <vector>

#include "CFG.h"

struct Production {
    std::string From;
    std::string Input;
    std::string Stacktop;
    std::string To;
    std::vector<std::string> Replacement;

    Production(std::string from, std::string input, std::string stacktop, std::string to,
               const std::vector<std::string> &replacement);
    std::string toString() const;
    void print();
};

class PDA {
private:
    std::set<std::string> Alphabet;
    std::set<std::string> StackAlphabet;
    std::set<std::string> States;
    std::string StartStack;
    std::string StartState;
    std::string productionsToString() const;
    std::vector<Production> Productions;
public:
    PDA(const std::string& inputjson);

    std::vector<std::vector<std::string>> getRHSCombinations(const std::vector<std::string> &Y, size_t current_index,
                                                             const std::string &r_prev,
                                                             const std::string &r_target) const;

    CFG toCFG() const;
    std::string to_string() const;
    void print() const;
};



#endif //PDA_H
