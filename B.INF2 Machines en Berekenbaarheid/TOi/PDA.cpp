#include "PDA.h"
#include <fstream>
#include "CFG.h"
#include "json.hpp"
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>


Production::Production(std::string from, std::string input, std::string stacktop,
                       std::string to, const std::vector<std::string> &replacement): From(std::move(from)),
    Input(std::move(input)),
    Stacktop(std::move(stacktop)),
    To(std::move(to)),
    Replacement(replacement) {
}

template<typename T>
std::string vectorToString(const std::vector<T> &v) {
    std::stringstream ss;
    ss << "[";
    const bool is_string = std::is_same<T, std::string>::value;
    for (size_t i = 0; i < v.size(); ++i) {
        if (is_string) {
            ss << "\"" << v[i] << "\"";
        } else {
            ss << v[i];
        }
        if (i < v.size() - 1) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}


template<typename T>
std::string setToString(const std::set<T> &v) {
    std::stringstream ss;
    ss << "[";
    const bool is_string = std::is_same<T, std::string>::value;
    for (auto it = v.begin(); it != v.end(); ++it) {
        if (is_string) {
            ss << "\"" << *it << "\"";
        } else {
            ss << *it;
        }
        if (std::next(it) != v.end()) {
            ss << ", ";
        }
    }
    ss << "]";
    return ss.str();
}

std::string Production::toString() const {
    return R"({"from", ")" + From + R"(", "input": ")" + Input + R"(", "stacktop": ")" + Stacktop + R"(", "to": ")" + To
           + R"(", "replacement": )" + vectorToString(Replacement) + "}";
}

PDA::PDA(const std::string &inputjson) {
    using json = nlohmann::json;

    try {
        std::ifstream input(inputjson);
        nlohmann::json j;
        input >> j;


        if (j.contains("States") && j["States"].is_array()) {
            for (const auto &state: j["States"]) {
                if (state.is_string()) {
                    States.insert(state.get<std::string>());
                }
            }
        }

        if (j.contains("Alphabet") && j["Alphabet"].is_array()) {
            for (const auto &symbol: j["Alphabet"]) {
                if (symbol.is_string()) {
                    Alphabet.insert(symbol.get<std::string>());
                }
            }
        }

        if (j.contains("StackAlphabet") && j["StackAlphabet"].is_array()) {
            for (const auto &symbol: j["StackAlphabet"]) {
                if (symbol.is_string()) {
                    StackAlphabet.insert(symbol.get<std::string>());
                }
            }
        }

        if (j.contains("StartState") && j["StartState"].is_string()) {
            StartState = j["StartState"].get<std::string>();
        }

        if (j.contains("StartStack") && j["StartStack"].is_string()) {
            StartStack = j["StartStack"].get<std::string>();
        }

        if (j.contains("Transitions") && j["Transitions"].is_array()) {
            for (const auto &transition: j["Transitions"]) {
                if (transition.is_object()) {
                    std::string from = transition.value("from", "");
                    std::string input = transition.value("input", "");
                    std::string stacktop = transition.value("stacktop", "");
                    std::string to = transition.value("to", "");

                    std::vector<std::string> replacement;
                    if (transition.contains("replacement") && transition["replacement"].is_array()) {
                        replacement = transition["replacement"].get<std::vector<std::string> >();
                    }

                    Productions.emplace_back(from, input, stacktop, to, replacement);
                }
            }
        }
    } catch (const json::exception &e) {
        std::cerr << "JSON processing error: " << e.what() << std::endl;
    }
}

std::string PDA::productionsToString() const {
    std::vector<std::string> production_strings;
    for (const auto &production: Productions) {
        production_strings.push_back(production.toString());
    }
    std::sort(production_strings.begin(), production_strings.end());
    std::stringstream ss;
    for (size_t i = 0; i < production_strings.size(); ++i) {
        ss << "    " << production_strings[i];
        if (i < production_strings.size() - 1) {
            ss << ",\n";
        }
    }
    return ss.str();
}

std::string PDA::to_string() const {
    std::stringstream ss;
    ss << "{\n";
    ss << "  \"States\": " << setToString(States) << ",\n";
    ss << "  \"Alphabet\": " << setToString(Alphabet) << ",\n";
    ss << "  \"StackAlphabet\": " << setToString(StackAlphabet) << ",\n";
    ss << productionsToString() << "\n";
    ss << "  ],\n";
    ss << R"(  "StartState": ")" << StartState << "\",\n";
    ss << R"(  "StartStack": ")" << StartStack << "\"\n";
    ss << "}";

    return ss.str();
}

void PDA::print() const {
    std::cout << to_string() << std::endl;
}

std::string buildName(const std::string &p, const std::string &X, const std::string &q) {
    return "[" + p + "," + X + "," + q + "]";
}

const std::string EPSILON_SYMBOL = "";

std::vector<std::vector<std::string> > PDA::getRHSCombinations(
    const std::vector<std::string> &Y,
    size_t current_index,
    const std::string &r_prev,
    const std::string &r_target
) const {
    std::vector<std::vector<std::string> > result;

    if (current_index == Y.size() - 1) {
        result.push_back({buildName(r_prev, Y[current_index], r_target)});
        return result;
    }

    // Recursive Case (Intermediate symbol Yi, i < k-1)
    for (const auto &r_next: this->States) {
        const std::string current_var = buildName(r_prev, Y[current_index], r_next);

        std::vector<std::vector<std::string> > tails = this->getRHSCombinations(
            Y,
            current_index + 1,
            r_next,
            r_target
        );

        for (std::vector<std::string> &tail: tails) {
            std::vector<std::string> new_rhs;
            new_rhs.push_back(current_var);
            new_rhs.insert(new_rhs.end(), tail.begin(), tail.end());
            result.push_back(new_rhs);
        }
    }
    return result;
}

CFG PDA::toCFG() const {
    // 1. Variables (V)
    std::set<std::string> variables;
    for (const auto &p: this->States) {
        for (const auto &q: this->States) {
            for (const auto &X: this->StackAlphabet) {
                variables.emplace(buildName(p, X, q));
            }
        }
    }
    const std::string S = "S";
    variables.insert(S);

    // 2. Terminals (T)
    std::set<std::string> terminals = Alphabet;

    // 3. Productions (P)
    std::unordered_map<std::string, std::vector<std::vector<std::string> > > productions;

    // A. Rule 1: S -> [q0 Z0 p]
    std::vector<std::vector<std::string> > sRHSs;
    for (const auto &p: this->States) {
        sRHSs.push_back({buildName(this->StartState, this->StartStack, p)});
    }
    productions[S] = sRHSs;

    // B. Rules 2 & 3: Processing PDA transitions
    for (const auto &prod: this->Productions) {
        const std::string &q = prod.From;
        const std::string &a = prod.Input;
        const std::string &X = prod.Stacktop;
        const std::string &r = prod.To; // New state after pop (r in the rule)
        const std::vector<std::string> &Y = prod.Replacement;

        if (Y.empty()) {
            // Rule 3: [qXr] -> a
            const std::string LHS = buildName(q, X, r);
            std::vector<std::string> RHS;
            if (a != EPSILON_SYMBOL) {
                RHS.push_back(a);
            }
            productions[LHS].push_back(RHS);
        } else {
            // Rule 2: [q X rk] -> a [r Y1 r1][r1 Y2 r2] ... [rk-1 Yk rk]

            for (const auto &r_final: this->States) {
                // LHS is [q X rk]
                const std::string LHS = buildName(q, X, r_final);

                std::vector<std::vector<std::string> > variable_sequences =
                        this->getRHSCombinations(Y, 0, r, r_final);

                for (std::vector<std::string> &sequence: variable_sequences) {
                    if (a != EPSILON_SYMBOL) {
                        sequence.insert(sequence.begin(), a);
                    }
                    productions[LHS].push_back(sequence);
                }
            }
        }
    }

    // 4. Start Symbol (S)
    const std::string &startsymbol = S;

    return {variables, terminals, productions, startsymbol};
}
