#include "ShuntingYard.h"

#include "ast/OperatorEnvironment.h"
#include "utils/Tokenizer.h"

#include <string>

namespace ShuntingYard {
std::queue<std::string> shuntingYard(const std::string& input, const cfg::OperatorConfig& ops) {
    const auto              tokens = Tokenize::tokenizeForParser(input, ops);
    std::queue<std::string> outputQueue;
    std::stack<std::string> operatorStack;

    for (const std::string& token : tokens) {
        if (std::isdigit(token[0]) || (token.size() > 1 && std::isdigit(token[1]))) {
            outputQueue.push(token); // Numbers go to output
        } else if (token == "(") {
            operatorStack.push(token);
        } else if (token == ")") {
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                outputQueue.push(operatorStack.top());
                operatorStack.pop();
            }
            if (!operatorStack.empty())
                operatorStack.pop(); // Remove "("
        } else {
            // Token is an operator
            while (!operatorStack.empty() && operatorStack.top() != "(") {
                const auto& o1 = PRECEDENCE.at(token);
                const auto& o2 = PRECEDENCE.at(operatorStack.top());

                if ((!o1.rightAssociative && o1.precedence <= o2.precedence) ||
                    (o1.rightAssociative && o1.precedence < o2.precedence)) {
                    outputQueue.push(operatorStack.top());
                    operatorStack.pop();
                } else
                    break;
            }
            operatorStack.push(token);
        }
    }

    while (!operatorStack.empty()) {
        outputQueue.push(operatorStack.top());
        operatorStack.pop();
    }
    return outputQueue;
}

double evaluateRPN(std::queue<std::string>& rpn, const expr::OperatorEnvironment& env) {
    std::stack<double> values;

    while (!rpn.empty()) {
        std::string token = rpn.front();
        rpn.pop();

        if (std::isdigit(token[0]) || (token.size() > 1 && std::isdigit(token[1]))) {
            values.push(std::stod(token));
        } else if (env.hasBinary(token)) {
            const double b = values.top();
            values.pop();
            const double a = values.top();
            values.pop();
            values.push(env.applyBinary(token, a, b));
        } else if (env.hasUnary(token)) {
            const double a = values.top();
            values.pop();
            values.push(env.applyUnary(token, a));
        }
    }
    return values.top();
}
} // namespace ShuntingYard
