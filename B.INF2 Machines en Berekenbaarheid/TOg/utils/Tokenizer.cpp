#include "Tokenizer.h"

#include "../config/OperatorConfig.h"

#include <algorithm>
#include <cctype>
#include <string>
#include <vector>

namespace Tokenize {

static bool isIdentStart(char c) { return std::isalpha(static_cast<unsigned char>(c)) || c == '_'; }

static bool isIdentChar(char c) { return std::isalnum(static_cast<unsigned char>(c)) || c == '_'; }

static size_t skipWs(const std::string& s, size_t i) {
    while (i < s.size() && std::isspace(static_cast<unsigned char>(s[i])))
        ++i;
    return i;
}

std::vector<std::string> tokenizeToCykSymbols(const std::string&         input,
                                              const cfg::OperatorConfig& ops) {
    std::vector<std::string> out;

    // operator lexemes longest-first
    std::vector<std::string> opLex(ops.allOperatorLexemes().begin(),
                                   ops.allOperatorLexemes().end());
    std::sort(opLex.begin(), opLex.end(),
              [](const std::string& a, const std::string& b) { return a.size() > b.size(); });

    bool prevCanEndExpr = false;

    auto pushToken      = [&](const std::string& tok) {
        out.push_back(tok);

        if (tok == "NUM" || tok == "ID" || tok == ")" || tok == "OP_UN_POST")
            prevCanEndExpr = true;
        else if (tok == "(" || tok == "," || tok == "OP_BIN" || tok == "OP_UN_PRE")
            prevCanEndExpr = false;
        else if (tok == "FUNC1" || tok == "FUNC2")
            prevCanEndExpr = false;
        else
            prevCanEndExpr = false;
    };

    for (size_t i = 0; i < input.size();) {
        i = skipWs(input, i);
        if (i >= input.size())
            break;

        char c = input[i];

        // Punctuation
        if (c == '(') {
            pushToken("(");
            ++i;
            continue;
        }
        if (c == ')') {
            pushToken(")");
            ++i;
            continue;
        }
        if (c == ',') {
            pushToken(",");
            ++i;
            continue;
        }

        // Number
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            bool   seenDot = (c == '.');
            size_t j       = i + 1;
            while (j < input.size()) {
                char d = input[j];
                if (std::isdigit(static_cast<unsigned char>(d))) {
                    ++j;
                    continue;
                }
                if (d == '.' && !seenDot) {
                    seenDot = true;
                    ++j;
                    continue;
                }
                break;
            }
            pushToken("NUM");
            i = j;
            continue;
        }

        // Identifier / named operators / functions
        if (isIdentStart(c)) {
            size_t j = i + 1;
            while (j < input.size() && isIdentChar(input[j]))
                ++j;
            std::string word         = input.substr(i, j - i);

            size_t      k            = skipWs(input, j);
            bool        nextIsLParen = (k < input.size() && input[k] == '(');

            if (nextIsLParen && word.size() > 1 && ops.isFunc2(word)) {
                pushToken("FUNC2");
            } else if (nextIsLParen && word.size() > 1 && ops.isFunc1(word)) {
                pushToken("FUNC1");
            } else {
                bool isPost = ops.isPostfix(word);
                bool canBin = ops.isBinary(word);
                bool canUn  = ops.isUnary(word);

                if (isPost && prevCanEndExpr)
                    pushToken("OP_UN_POST");
                else if (canBin && canUn)
                    pushToken(prevCanEndExpr ? "OP_BIN" : "OP_UN_PRE");
                else if (canBin)
                    pushToken("OP_BIN");
                else if (canUn)
                    pushToken("OP_UN_PRE");
                else
                    pushToken("ID");
            }

            i = j;
            continue;
        }

        // Symbol operators: longest match
        bool matched = false;
        for (const auto& lex : opLex) {
            if (lex.empty())
                continue;
            if (i + lex.size() <= input.size() && input.compare(i, lex.size(), lex) == 0) {
                if (ops.isFunc2(lex)) {
                    pushToken("FUNC2");
                } else if (ops.isFunc1(lex)) {
                    pushToken("FUNC1");
                } else {
                    bool isPost = ops.isPostfix(lex);
                    bool canBin = ops.isBinary(lex);
                    bool canUn  = ops.isUnary(lex);

                    if (isPost && prevCanEndExpr)
                        pushToken("OP_UN_POST");
                    else if (canBin && canUn)
                        pushToken(prevCanEndExpr ? "OP_BIN" : "OP_UN_PRE");
                    else if (canBin)
                        pushToken("OP_BIN");
                    else if (canUn)
                        pushToken("OP_UN_PRE");
                    else
                        pushToken("ID");
                }

                i += lex.size();
                matched = true;
                break;
            }
        }
        if (matched)
            continue;

        // Unknown char
        pushToken("ID");
        ++i;
    }

    return out;
}

std::vector<std::string> tokenizeForParser(const std::string&         input,
                                           const cfg::OperatorConfig& ops) {
    std::vector<std::string> out;

    std::vector<std::string> opLex(ops.allOperatorLexemes().begin(),
                                   ops.allOperatorLexemes().end());
    std::sort(opLex.begin(), opLex.end(),
              [](const std::string& a, const std::string& b) { return a.size() > b.size(); });

    for (size_t i = 0; i < input.size();) {
        i = skipWs(input, i);
        if (i >= input.size())
            break;

        char c = input[i];

        if (c == '(') {
            out.push_back("(");
            ++i;
            continue;
        }
        if (c == ')') {
            out.push_back(")");
            ++i;
            continue;
        }
        if (c == ',') {
            out.push_back(",");
            ++i;
            continue;
        }

        // number literal kept as-is
        if (std::isdigit(static_cast<unsigned char>(c)) || c == '.') {
            bool   seenDot = (c == '.');
            size_t j       = i + 1;
            while (j < input.size()) {
                char d = input[j];
                if (std::isdigit(static_cast<unsigned char>(d))) {
                    ++j;
                    continue;
                }
                if (d == '.' && !seenDot) {
                    seenDot = true;
                    ++j;
                    continue;
                }
                break;
            }
            out.push_back(input.substr(i, j - i));
            i = j;
            continue;
        }

        if (isIdentStart(c)) {
            size_t j = i + 1;
            while (j < input.size() && isIdentChar(input[j]))
                ++j;

            std::string word = input.substr(i, j - i);
            out.push_back(word);

            size_t k            = skipWs(input, j);
            bool   nextIsLParen = (k < input.size() && input[k] == '(');

            if (nextIsLParen && (ops.isFunc1(word) || ops.isFunc2(word))) {
                out.push_back("(");
                i = k + 1;
            } else {
                i = j;
            }
            continue;
        }

        // operator lexeme longest match, kept as-is
        bool matched = false;
        for (const auto& lex : opLex) {
            if (lex.empty())
                continue;
            if (i + lex.size() <= input.size() && input.compare(i, lex.size(), lex) == 0) {
                out.push_back(lex);
                i += lex.size();
                matched = true;
                break;
            }
        }
        if (matched)
            continue;

        // fallback single char
        out.emplace_back(1, c);
        ++i;
    }

    return out;
}
} // namespace Tokenize
