#include "RE.h"
#include <iostream>
#include <algorithm>
#include <vector>
namespace Norkella {
RE::RE() : expression("") {}

RE::RE(const std::string& expr) : expression(expr) {}

void RE::print() const {
    std::cout << expression << std::endl;
}


const std::string& RE::str() const {
    return expression;
}

RE RE::concat(const RE& a, const RE& b) {
    if (a.expression.empty()) return b;
    if (b.expression.empty()) return a;

    std::string ae = a.expression;
    std::string be = b.expression;

    if (ae.front() == '(' && ae.back() == ')' && ae.find('+') == std::string::npos) {
        ae = ae.substr(1, ae.size() - 2);
    }
    if (be.front() == '(' && be.back() == ')' && be.find('+') == std::string::npos) {
        be = be.substr(1, be.size() - 2);
    }

    return RE(ae + be);
}

static std::vector<std::string> flattenUnion(const std::string& expr) {
    std::vector<std::string> parts;

    if (expr.empty()) return parts;

    std::string core = expr;
    if (core.front() == '(' && core.back() == ')') {
        core = core.substr(1, core.size() - 2); // remove outer ()
    }

    std::string token;
    int depth = 0;
    for (char c : core) {
        if (c == '(') depth++;
        if (c == ')') depth--;
        if (c == '+' && depth == 0) {
            if (!token.empty()) {
                parts.push_back(token);
                token.clear();
            }
        } else {
            token += c;
        }
    }
    if (!token.empty()) parts.push_back(token);
    std::sort(parts.begin(), parts.end());
    return parts;
}


RE RE::unionRE(const RE& a, const RE& b) {
    if (a.expression.empty()) return b;
    if (b.expression.empty()) return a;

    std::vector<std::string> parts = flattenUnion(a.expression);
    std::vector<std::string> partsB = flattenUnion(b.expression);
    parts.insert(parts.end(), partsB.begin(), partsB.end());

    std::sort(parts.begin(), parts.end());
    parts.erase(std::unique(parts.begin(), parts.end()), parts.end());

    std::string result = parts[0];
    for (size_t i = 1; i < parts.size(); ++i) {
        result += "+" + parts[i];
    }

    return RE("(" + result + ")");
}
RE RE::star(const RE& a) {
    if (a.str().empty()) return RE("");
    return RE(a.str() + "*");
}
}