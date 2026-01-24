#include "RE.h"

#include <cctype>
#include <stack>
#include <map>
#include <set>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <sstream>

namespace Adriaan {

static constexpr char ESC = '\x01';

//  ────────────────────────────────────────────────
//  Helpers for validation / diagnostics
//  ────────────────────────────────────────────────

namespace {
    [[nodiscard]] bool isLiteral(char c) {
        static const std::string operators = "*+.()#";
        unsigned char uc = static_cast<unsigned char>(c);
        return std::isspace(uc) || (uc >= 32 && uc <= 126 && operators.find(c) == std::string::npos);
    }

    [[nodiscard]] std::string positionIndicator(const std::string& txt, std::size_t pos) {
        std::ostringstream oss;
        oss << txt << "\n" << std::string(pos, ' ') << "^";
        return oss.str();
    }

    void expect(bool cond, const std::string& msg) {
        if (!cond) throw std::runtime_error(msg);
    }

    template <class Stack>
    auto popOrThrow(Stack& st, const std::string& errMsg) {
        if (st.empty()) throw std::runtime_error(errMsg);
        auto top = std::move(st.top());
        st.pop();
        return top;
    }
}

RE::RE(const std::string& expr, char eps) : expression(expr), epsilon(eps) {}

bool RE::isLiteral(char c) {
    static const std::string operators = "*+.()#";

    unsigned char uc = static_cast<unsigned char>(c);

    return std::isspace(uc) || (uc >= 32 && uc <= 126 && operators.find(c) == std::string::npos);
}

ENFA RE::toENFA() const {
  if (expression.empty())
    throw std::runtime_error(
        "Lege regex opgegeven – er is niets om te converteren.");

  //--------------------------------------------------------------------------
  // 1) Expand '^' → '#'
  //--------------------------------------------------------------------------
  std::string expanded;
  expanded.reserve(expression.size());

  for (std::size_t i = 0; i < expression.size(); ++i) {
    if (expression[i] == '\\' && i + 1 < expression.size()) {
      expanded += expression[i++];
      expanded += expression[i];
    } else if (expression[i] == '^') {
      expanded += '#';
    } else {
      expanded += expression[i];
    }
  }

  //--------------------------------------------------------------------------
  // 1a) Quick syntax validation (balanced parentheses, disallow empty escapes)
  //--------------------------------------------------------------------------
  {
    int depth = 0;
    for (std::size_t i = 0; i < expanded.size(); ++i) {
      char c = expanded[i];
      if (c == '(')
        ++depth;
      else if (c == ')')
        --depth;
      else if (c == '\\') {
        expect(i + 1 < expanded.size(),
               "Ongeldige escape‑sequentie op het einde van de regex.\n" +
                   positionIndicator(expanded, i));
        ++i; // skip escaped char
      }
      expect(depth >= 0,
             "Ongebalanceerde haakjes ')'.\n" + positionIndicator(expanded, i));
    }
    expect(depth == 0, "Ongebalanceerde haakjes '('.\n" +
                           positionIndicator(expanded, expanded.size()));
  }

  //--------------------------------------------------------------------------
  // 2) Tokenise – keep "#*" atomic, ESC prefixed literals as one token
  //--------------------------------------------------------------------------
  std::vector<std::string> tokens;
  tokens.reserve(expanded.size());

  for (std::size_t i = 0; i < expanded.size(); ++i) {
    if (expanded[i] == '\\' && i + 1 < expanded.size()) {
      tokens.emplace_back(std::string{ESC, expanded[i + 1]});
      ++i;
    } else if (expanded[i] == '#' && i + 1 < expanded.size() &&
               expanded[i + 1] == '*') {
      tokens.emplace_back("#*");
      ++i;
    } else {
      tokens.emplace_back(1, expanded[i]);
    }
  }

  expect(!tokens.empty(),
         "Na tokenisatie bleven er geen tokens over – ongeldige regex?");

  //--------------------------------------------------------------------------
  // 3) Insert explicit concatenation '.'
  //--------------------------------------------------------------------------
  std::string formatted;
  formatted.reserve(tokens.size() * 2);

  for (std::size_t i = 0; i < tokens.size(); ++i) {
    formatted += tokens[i];
    if (i + 1 >= tokens.size())
      continue;

    const std::string &c1 = tokens[i];
    const std::string &c2 = tokens[i + 1];

    bool left = (c1 != "." && c1 != "+" && c1 != "(");
    bool right = (c2 != "." && c2 != "+" && c2 != "*" && c2 != ")");

    if ((c1 == "#*" && right) || (c1 == ")" && right) || (left && c2 == "(") ||
        (left && right)) {
      formatted += '.';
    }
  }

  //--------------------------------------------------------------------------
  // 4) Shunting‑yard → infix → postfix
  //--------------------------------------------------------------------------
  std::string postfix;
  postfix.reserve(formatted.size());

  std::stack<char> ops;
  std::map<char, int> prec{{'*', 3}, {'.', 2}, {'+', 1}, {'(', 0}};

  for (std::size_t i = 0; i < formatted.size(); ++i) {
    char c = formatted[i];

    // Multi‑char tokens first
    if (c == ESC && i + 1 < formatted.size()) {
      postfix += ESC;
      postfix += formatted[++i];
      continue;
    }
    if (c == '#' && i + 1 < formatted.size() && formatted[i + 1] == '*') {
      postfix += "#*";
      ++i;
      continue;
    }

    if (isLiteral(c) || c == '#') {
      postfix += c;
    } else if (c == '(') {
      ops.push(c);
    } else if (c == ')') {
      expect(!ops.empty(),
             "')' zonder bijhorende '('.\n" + positionIndicator(formatted, i));
      while (!ops.empty() && ops.top() != '(') {
        postfix += ops.top();
        ops.pop();
      }
      expect(!ops.empty() && ops.top() == '(',
             "')' zonder bijhorende '('.\n" + positionIndicator(formatted, i));
      ops.pop(); // remove '('
    } else {
      // operator * . +
      while (!ops.empty() && ops.top() != '(' && prec[ops.top()] >= prec[c]) {
        postfix += ops.top();
        ops.pop();
      }
      ops.push(c);
    }
  }
  while (!ops.empty()) {
    expect(ops.top() != '(',
           "Ongebalanceerde '('.\n" +
               positionIndicator(formatted, formatted.size()));
    postfix += ops.top();
    ops.pop();
  }

  //--------------------------------------------------------------------------
  // 5) Postfix evaluation
  //--------------------------------------------------------------------------
  std::stack<ENFA> st;
  int nextStateId = 0;

  auto freshState = [&]() {
    if (nextStateId == std::numeric_limits<int>::max())
      throw std::runtime_error("State ID‑overflow – extreem grote automaat.");
    return nextStateId++;
  };

  for (std::size_t i = 0; i < postfix.size(); ++i) {
    char tok = postfix[i];

    //------------------------------------------------------------------
    //  ESC‑prefixed literal
    //------------------------------------------------------------------
    if (tok == ESC && i + 1 < postfix.size()) {
      char lit = postfix[++i];
      ENFA nfa(epsilon);
      int s = freshState();
      int f = freshState();
      nfa.setStartState(s);
      nfa.addAcceptingState(f);
      nfa.addTransition(s, lit, f);
      st.push(std::move(nfa));
      continue;
    }
    //------------------------------------------------------------------
    //  #*  (wildcard Kleene star) – two‑char token in postfix too
    //------------------------------------------------------------------
    if (tok == '#' && i + 1 < postfix.size() && postfix[i + 1] == '*') {
      ENFA nfa(epsilon);
      int s = freshState();
      int f = freshState();
      nfa.setStartState(s);
      nfa.addAcceptingState(f);
      for (char c = 32; c <= 126; ++c) {
        nfa.addTransition(s, c, f);
        nfa.addTransition(f, c, f);
      }
      nfa.addTransition(s, epsilon, f);
      st.push(std::move(nfa));
      ++i; // skip '*'
      continue;
    }
    //------------------------------------------------------------------
    //  Single‑char literal or '#'
    //------------------------------------------------------------------
    if (isLiteral(tok)) {
      ENFA nfa(epsilon);
      int s = freshState();
      int f = freshState();
      nfa.setStartState(s);
      nfa.addAcceptingState(f);
      nfa.addTransition(s, tok, f);
      st.push(std::move(nfa));
      continue;
    }
    if (tok == '#') {
      ENFA nfa(epsilon);
      int s = freshState();
      int f = freshState();
      nfa.setStartState(s);
      nfa.addAcceptingState(f);
      for (char c = 32; c <= 126; ++c) {
        nfa.addTransition(s, c, f);
      }
      st.push(std::move(nfa));
      continue;
    }
    //------------------------------------------------------------------
    //  Operators – need 1 (*), 2 (., +) operands respectively
    //------------------------------------------------------------------
    if (tok == '*') {
      ENFA nfa =
          popOrThrow(st, "Operator '*' eist 1 operand maar de stack was leeg.");
      ENFA res(epsilon);
      int s = freshState();
      int f = freshState();
      res.setStartState(s);
      res.addAcceptingState(f);
      res.addTransition(s, epsilon, f);
      res.addTransition(s, epsilon, nfa.startState);
      for (const auto &[key, tgt] : nfa.transitions) {
        for (int to : tgt)
          res.addTransition(key.first, key.second, to);
      }
      for (int a : nfa.acceptingStates) {
        res.addTransition(a, epsilon, f);
        res.addTransition(a, epsilon, nfa.startState);
      }
      st.push(std::move(res));
    } else if (tok == '.' || tok == '+') {
      ENFA nfa2 =
          popOrThrow(st, std::string("Operator '") + tok +
                             "' eist twee operanden maar vond er minder.");
      ENFA nfa1 =
          popOrThrow(st, std::string("Operator '") + tok +
                             "' eist twee operanden maar vond er minder.");

      ENFA res(epsilon);
      if (tok == '.') {
        // concatenation
        for (int s : nfa1.states)
          res.states.insert(s);
        for (int s : nfa2.states)
          res.states.insert(s);
        for (const auto &[key, tgt] : nfa1.transitions)
          for (int to : tgt)
            res.addTransition(key.first, key.second, to);
        for (const auto &[key, tgt] : nfa2.transitions)
          for (int to : tgt)
            res.addTransition(key.first, key.second, to);
        for (int a : nfa1.acceptingStates)
          res.addTransition(a, epsilon, nfa2.startState);
        res.setStartState(nfa1.startState);
        res.acceptingStates = nfa2.acceptingStates;
      } else {
        // alternation '+'
        int s = freshState();
        int f = freshState();
        res.setStartState(s);
        res.addAcceptingState(f);
        res.addTransition(s, epsilon, nfa1.startState);
        res.addTransition(s, epsilon, nfa2.startState);
        for (int a : nfa1.acceptingStates)
          res.addTransition(a, epsilon, f);
        for (int a : nfa2.acceptingStates)
          res.addTransition(a, epsilon, f);
        for (int stId : nfa1.states)
          res.states.insert(stId);
        for (int stId : nfa2.states)
          res.states.insert(stId);
        for (const auto &[key, tgt] : nfa1.transitions)
          for (int to : tgt)
            res.addTransition(key.first, key.second, to);
        for (const auto &[key, tgt] : nfa2.transitions)
          for (int to : tgt)
            res.addTransition(key.first, key.second, to);
      }
      st.push(std::move(res));
    } else {
      throw std::runtime_error(std::string("Onbekende token '") + tok +
                               "' in postfix‐expressie.");
    }
  }

  //--------------------------------------------------------------------------
  // 6) Final sanity checks and alphabet synthesis
  //--------------------------------------------------------------------------
  expect(
      !st.empty(),
      "Postfix‑evaluatie faalde: stack is leeg. Vermoedelijk ongeldige regex.");
  ENFA result = popOrThrow(
      st, "Interne fout: lege stack terwijl er één resultaat verwacht werd.");
  expect(st.empty(),
         "Postfix‑evaluatie faalde: er bleven nog waarden op de stack staan.");

  std::set<char> alphabet;
  for (const auto &[key, _] : result.transitions) {
    if (key.second != epsilon)
      alphabet.insert(key.second);
  }
  result.setAlphabet(alphabet);

  std::cout << "========  toENFA() done (" << result.states.size()
            << " states)  ========\n";
  return result;
}
std::string RE::getExpression() const { return expression; }
char RE::getEpsilon() const { return epsilon; }
} // namespace Adriaan