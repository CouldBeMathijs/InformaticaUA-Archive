#include "RE.h"

#include <iostream>
#include <stack>

RE::RE(const std::string &regex, char epsilon)
    : regex(regex), epsilon(epsilon) {}

ENFA RE::toENFA() const {
    std::cerr << "RE -> to eNFA is not (yet) implemented" << std::endl;
    ENFA enfa;
    std::stack<std::pair<std::string, std::string>> stack; // Stack to keep track of start and end states of sub-NFAs
    std::stack<size_t> parenStack; // Stack to keep track of '(' positions
    unsigned int count = 0; // Counter for generating unique state names
    enfa.setEpsilon(epsilon); // Set epsilon character

    // If the regex is empty, return an empty ENFA
    if (regex.empty()) {
        return enfa;
    }

    std::unordered_map<std::string, std::unordered_map<char, std::unordered_set<std::string>>> transition_table;

    // Helper function to handle implicit concatenation
    auto handleImplicitConcatenation = [&]() {
        if (stack.size() >= 2) {
            auto b = stack.top();
            stack.pop();
            auto a = stack.top();
            stack.pop();

            // Connect the end state of 'a' to the start state of 'b' via epsilon
            transition_table[a.second][epsilon].insert(b.first);

            // Push the start state of 'a' and the end state of 'b' onto the stack
            stack.push({a.first, b.second});
        }
    };

    // Traverse the regex
    for (size_t i = 0; i < regex.size(); ++i) {
        char c = regex[i];

        switch (c) {
            case '(':
                // Handle implicit concatenation before opening parenthesis
                if (i > 0 && (isalnum(regex[i - 1]) || regex[i - 1] == ')' || regex[i - 1] == '*')) {
                    handleImplicitConcatenation();
                }
                // Push the position of the '(' onto the paren stack
                parenStack.push(stack.size());
                break;

            case ')':
                // Handle implicit concatenation before closing parenthesis
                if (i > 0 && (isalnum(regex[i - 1]) || regex[i - 1] == ')' || regex[i - 1] == '*')) {
                    handleImplicitConcatenation();
                }
                // Handle closing parenthesis, pop the stack
                if (!parenStack.empty()) {
                    size_t openIndex = parenStack.top();
                    parenStack.pop();
                    if (stack.size() > openIndex) {
                        auto subNFA = stack.top();
                        stack.pop();
                        stack.emplace(subNFA.first, subNFA.second);
                    }
                }
                break;

            case '*': {  // Kleene star
                // Handle implicit concatenation before Kleene star
                if (i > 0 && (isalnum(regex[i - 1]) || regex[i - 1] == ')' || regex[i - 1] == '*')) {
                    handleImplicitConcatenation();
                }
                if (stack.empty()) {
                    continue;
                }

                auto a = stack.top();
                stack.pop();

                // Create new start and end states for the Kleene star
                std::string new_start = "q" + std::to_string(count++);
                std::string new_end = "q" + std::to_string(count++);

                // Add epsilon transitions for the Kleene star
                transition_table[new_start][epsilon].insert(a.first); // New start -> Old start
                transition_table[new_start][epsilon].insert(new_end); // New start -> New end (zero repeats)
                transition_table[a.second][epsilon].insert(a.first); // Old end -> Old start (repetition)
                transition_table[a.second][epsilon].insert(new_end); // Old end -> New end

                // Push the new start and end states onto the stack
                stack.push({new_start, new_end});
                break;
            }

            case '+': {  // Union (alternation)
                // Handle implicit concatenation before union
                if (i > 0 && (isalnum(regex[i - 1]) || regex[i - 1] == ')' || regex[i - 1] == '*')) {
                    handleImplicitConcatenation();
                }
                if (stack.size() < 2) {
                    continue;
                }

                auto b = stack.top();
                stack.pop();
                auto a = stack.top();
                stack.pop();

                // Create new start and end states for the union
                std::string new_start = "q" + std::to_string(count++);
                std::string new_end = "q" + std::to_string(count++);

                // Add epsilon transitions for the union
                transition_table[new_start][epsilon].insert(a.first); // New start -> Start of 'a'
                transition_table[new_start][epsilon].insert(b.first); // New start -> Start of 'b'
                transition_table[a.second][epsilon].insert(new_end); // End of 'a' -> New end
                transition_table[b.second][epsilon].insert(new_end); // End of 'b' -> New end

                // Push the new start and end states onto the stack
                stack.push({new_start, new_end});
                break;
            }

            default: {  // Character operand
                // Handle implicit concatenation before character
                if (i > 0 && (isalnum(regex[i - 1]) || regex[i - 1] == ')' || regex[i - 1] == '*')) {
                    handleImplicitConcatenation();
                }
                std::string start = "q" + std::to_string(count++);
                std::string accept = "q" + std::to_string(count++);

                // Add the transition for the character
                transition_table[start][c].insert(accept);
                stack.push({start, accept});

                // If this is the first state, set it as the start state
                if (transition_table.size() == 1) {
                    enfa.setStartState(start);
                }
                break;
            }
        }
    }

    // Handle any remaining implicit concatenation
    while (stack.size() >= 2) {
        handleImplicitConcatenation();
    }

    // If the stack is not empty, set the accepting state
    if (!stack.empty()) {
        auto finalState = stack.top();
        stack.pop();
        enfa.setAcceptingStates({finalState.second});
    }

    // Set the final transition table
    enfa.setTransitionTable(transition_table);

    return enfa;
}