#include "SLRParser.h"

#include <iomanip>
#include <queue>

static std::unique_ptr<expr::ASTNode> insertBinaryWithPrecedence(
    const std::string& op, std::unique_ptr<expr::ASTNode> left,
    std::unique_ptr<expr::ASTNode> right, const cfg::OperatorConfig* ops) {
    if (!ops)
        return std::make_unique<expr::CustomBinaryOpNode>(op, std::move(left), std::move(right));

    if (!ops->isBinary(op))
        return std::make_unique<expr::CustomBinaryOpNode>(op, std::move(left), std::move(right));

    auto* leftBin = dynamic_cast<expr::CustomBinaryOpNode*>(left.get());
    if (!leftBin)
        return std::make_unique<expr::CustomBinaryOpNode>(op, std::move(left), std::move(right));

    if (!ops->isBinary(leftBin->symbol()))
        return std::make_unique<expr::CustomBinaryOpNode>(op, std::move(left), std::move(right));

    const int pNew  = ops->binaryWeight(op);
    const int pLeft = ops->binaryWeight(leftBin->symbol());

    if (const bool newRightAssoc = ops->binaryRightAssoc(op);
        pNew > pLeft || (pNew == pLeft && newRightAssoc)) {
        auto B        = leftBin->releaseRight();
        auto newRight = insertBinaryWithPrecedence(op, std::move(B), std::move(right), ops);
        leftBin->setRight(std::move(newRight));
        return left;
    }

    return std::make_unique<expr::CustomBinaryOpNode>(op, std::move(left), std::move(right));
}

LR0Item::LR0Item(std::string h, std::vector<std::string> b, const int dot)
    : head(std::move(h))
    , body(std::move(b))
    , dotPos(dot) {}

std::string LR0Item::getSymbolAfterDot() const {
    if (dotPos < body.size())
        return body[dotPos];
    return "";
}

LR0Item LR0Item::getNextItem() const { return {head, body, dotPos + 1}; }

bool    LR0Item::isReduceItem() const { return dotPos >= body.size(); }

bool    LR0Item::operator<(const LR0Item& other) const {
    if (head != other.head)
        return head < other.head;
    if (body != other.body)
        return body < other.body;
    return dotPos < other.dotPos;
}

bool LR0Item::operator==(const LR0Item& other) const {
    return head == other.head && body == other.body && dotPos == other.dotPos;
}

bool LR0State::operator==(const LR0State& other) const { return items == other.items; }

// Helper to get productions
std::vector<std::pair<std::string, std::vector<std::string>>> SLRParser::getProductionsVector()
    const {
    std::vector<std::pair<std::string, std::vector<std::string>>> result;
    for (const auto& prod : cfg.getProductions())
        result.push_back(prod);
    return result;
}

void SLRParser::computeFirstSets() {
    const auto& variables   = cfg.getVariables();
    const auto& terminals   = cfg.getTerminals();
    auto        productions = getProductionsVector();

    // Init
    for (const auto& var : variables)
        firstSets[var] = {};
    for (const auto& term : terminals)
        firstSets[term] = {term};

    bool changed;
    do {
        changed = false;
        for (const auto& prod : productions) {
            const std::string&              head = prod.first;
            const std::vector<std::string>& body = prod.second;

            if (body.empty()) { // Epsilon
                if (firstSets[head].insert("").second)
                    changed = true;
            } else {
                auto firstOfBody = computeFirst(body);
                for (const auto& symbol : firstOfBody)
                    if (firstSets[head].insert(symbol).second)
                        changed = true;
            }
        }
    } while (changed);
}

void SLRParser::computeFollowSets() {
    const auto& variables   = cfg.getVariables();
    auto        productions = getProductionsVector();

    for (const auto& var : variables)
        followSets[var] = {};
    followSets[cfg.getStartSymbol()].insert("EOS");

    bool changed;
    do {
        changed = false;
        for (const auto& prod : productions) {
            const auto& body = prod.second;
            for (int i = 0; i < body.size(); ++i) {
                if (variables.contains(body[i])) { // If the symbol is a variable
                    std::vector<std::string> beta(body.begin() + i + 1, body.end());
                    auto                     firstBeta = computeFirst(beta);

                    for (const auto& symbol : firstBeta)
                        if (!symbol.empty() && followSets[body[i]].insert(symbol).second)
                            changed = true;

                    if (firstBeta.contains("") || beta.empty()) {
                        for (const auto& symbol : followSets[prod.first])
                            if (followSets[body[i]].insert(symbol).second)
                                changed = true;
                    }
                }
            }
        }
    } while (changed);
}

std::unordered_set<std::string> SLRParser::computeFirst(const std::vector<std::string>& symbols) {
    std::unordered_set<std::string> result;
    if (symbols.empty()) {
        result.insert("");
        return result;
    } // Empty string = epsilon

    bool allEpsilon = true;
    for (const auto& symbol : symbols) {
        if (cfg.getTerminals().contains(symbol)) {
            result.insert(symbol);
            allEpsilon = false;
            break;
        } else {
            const auto& f = firstSets[symbol];
            for (const auto& s : f)
                if (!s.empty())
                    result.insert(s);
            if (!f.contains("")) {
                allEpsilon = false;
                break;
            }
        }
    }
    if (allEpsilon)
        result.insert("");
    return result;
}

// CLOSURE & GOTO

void SLRParser::computeClosure(LR0State& state) const {
    const auto&         variables   = cfg.getVariables();
    auto                productions = getProductionsVector();

    std::queue<LR0Item> q;
    for (const auto& item : state.items)
        q.push(item);

    while (!q.empty()) {
        LR0Item curr = q.front();
        q.pop();
        std::string sym = curr.getSymbolAfterDot();

        if (variables.contains(sym)) {
            for (const auto& prod : productions) {
                if (prod.first == sym) {
                    LR0Item newItem(prod.first, prod.second, 0);
                    if (!state.items.contains(newItem)) {
                        state.items.insert(newItem);
                        q.push(newItem);
                    }
                }
            }
        }
    }
}

LR0State SLRParser::computeGoto(const LR0State& state, const std::string& symbol) const {
    LR0State newState;
    newState.stateId = -1;

    for (const auto& item : state.items)
        if (item.getSymbolAfterDot() == symbol)
            newState.items.insert(item.getNextItem());
    if (!newState.items.empty())
        computeClosure(newState);
    return newState;
}

std::string SLRParser::getAction(int state, const std::string& symbol) const {
    if (actionTable.contains(state) && actionTable.at(state).contains(symbol))
        return actionTable.at(state).at(symbol);
    if (gotoTable.contains(state) && gotoTable.at(state).contains(symbol))
        return std::to_string(gotoTable.at(state).at(symbol));
    return "";
}

void SLRParser::buildSLRTable(std::ostream& io) {
    io << ">>> Building SLR table..." << std::endl;

    // Calculate FIRST/FOLLOW sets
    computeFirstSets();
    computeFollowSets();

    auto                  productions  = getProductionsVector();
    const auto&           terminals    = cfg.getTerminals();
    std::set<std::string> allVariables = cfg.getVariables();
    allVariables.insert("S'");

    // 1. Initial State
    LR0State startState;
    startState.stateId = 0;
    startState.items.insert(LR0Item("S'", {cfg.getStartSymbol()}, 0));
    computeClosure(startState);
    canonicalCollection.push_back(startState);

    std::queue<int> q;
    q.push(0);

    while (!q.empty()) {
        int currID = q.front();
        q.pop();
        LR0State currState = canonicalCollection[currID];

        // Get all the symbols (terminals + variables)
        std::set<std::string> allSyms;
        for (const auto& t : terminals)
            allSyms.insert(t);
        for (const auto& v : allVariables)
            allSyms.insert(v);

        for (const auto& sym : allSyms) {
            LR0State nextState = computeGoto(currState, sym);
            if (nextState.items.empty())
                continue;

            // Check of state exists
            int nextID = -1;
            for (auto i = 0; i < canonicalCollection.size(); ++i) {
                if (canonicalCollection[i] == nextState) {
                    nextID = i;
                    break;
                }
            }

            // new state
            if (nextID == -1) {
                nextID            = static_cast<int>(canonicalCollection.size());
                nextState.stateId = nextID;
                canonicalCollection.push_back(nextState);
                q.push(nextID);
            }

            // Enter actions
            if (terminals.contains(sym))
                actionTable[currID][sym] = "s" + std::to_string(nextID);
            else if (allVariables.contains(sym))
                gotoTable[currID][sym] = nextID;
        }

        // Reduce & accept
        for (const auto& item : currState.items) {
            if (item.isReduceItem()) {
                if (item.head == "S'") {
                    actionTable[currID]["EOS"] = "acc";
                } else {
                    // Look for productions index
                    int  idx   = 0;
                    bool found = false;
                    for (int k = 0; k < productions.size(); ++k) {
                        if (productions[k].first == item.head &&
                            productions[k].second == item.body) {
                            idx   = k;
                            found = true;
                            break;
                        }
                    }

                    if (found) {
                        for (const auto& f : followSets[item.head]) {
                            if (!actionTable[currID].contains(f)) {
                                actionTable[currID][f] = "r" + std::to_string(idx);
                            } else {
                                io << "SLR CONFLICT: State " << currID << ", Symbol " << f << " - "
                                   << actionTable[currID][f] << " vs r" << idx << std::endl;
                            }
                        }
                    }
                }
            }
        }
    }
}

void SLRParser::printSLRTable(ostream& io) const {
    io << ">>> SLR Parsing Table (" << canonicalCollection.size() << " states)" << std::endl;
    io << "-------------------------------------" << std::endl;

    // Headers
    std::vector<std::string> headers;
    for (const auto& t : cfg.getTerminals())
        headers.push_back(t);
    headers.emplace_back("EOS");
    for (const auto& v : cfg.getVariables())
        if (v != "S'")
            headers.push_back(v);

    // Print header
    io << std::setw(6) << "State" << "|";
    for (const auto& h : headers)
        io << std::setw(8) << h << "|";
    io << std::endl;

    io << "------|";
    for (size_t i = 0; i < headers.size(); ++i)
        io << "---------|";
    io << std::endl;

    // Print rows
    for (int i = 0; i < canonicalCollection.size(); ++i) {
        io << std::setw(6) << i << "|";
        for (const auto& h : headers) {
            std::string action = getAction(i, h);
            io << std::setw(8) << action << "|";
        }
        io << std::endl;
    }
    io << "-------------------------------------" << std::endl;
}

void SLRParser::slr(std::ostream& io) {
    buildSLRTable(io);
    printSLRTable(io);
}

bool SLRParser::isNumber(const std::string& s) {
    if (s.empty())
        return false;

    bool hasDecimal = false;
    for (size_t i = 0; i < s.size(); ++i) {
        if (i == 0 && s[i] == '-') {
            if (s.size() == 1)
                return false;
            continue;
        }
        if (s[i] == '.') {
            if (hasDecimal)
                return false;
            hasDecimal = true;
        } else if (!std::isdigit(s[i])) {
            return false;
        }
    }
    return true;
}

bool SLRParser::isValidIdentifier(const std::string& s) {
    if (s.empty())
        return false;

    // First char has to be a letter or _
    if (!std::isalpha(s[0]) && s[0] != '_')
        return false;

    // The rest can be letters, numbers or _
    for (size_t i = 1; i < s.size(); ++i)
        if (!std::isalnum(s[i]) && s[i] != '_')
            return false;

    return true;
}

static std::string suggestFuncCall(const std::string& ident, const cfg::OperatorConfig* ops) {
    if (!ops)
        return "";

    auto check = [&](const std::set<std::string>& funcs) -> std::string {
        for (const auto& f : funcs) {
            if (ident.size() > f.size() && ident.rfind(f, 0) == 0 && // starts with f
                std::isdigit(static_cast<unsigned char>(ident[f.size()]))) {
                // split: f + restDigits
                return f + "(" + ident.substr(f.size()) + ")";
            }
        }
        return "";
    };

    std::string s = check(ops->func1);
    if (!s.empty())
        return s;
    s = check(ops->func2); // also works for func2, but will suggest f(rest)
    return s;
}

bool SLRParser::validateParentheses(const std::vector<std::string>& tokens, std::ostream& io) {
    int    balance   = 0;
    size_t firstOpen = 0;
    bool   foundOpen = false;

    for (size_t i = 0; i < tokens.size(); ++i) {
        if (tokens[i] == "(") {
            if (!foundOpen) {
                firstOpen = i;
                foundOpen = true;
            }
            balance++;
        } else if (tokens[i] == ")") {
            balance--;
            if (balance < 0) {
                io << "Error: Unmatched closing parenthesis ')' at position " << i << "\n";
                printInputWithPointer(tokens, i, io);
                return false;
            }
        }
    }

    if (balance > 0) {
        io << "Error: " << balance << " unclosed opening parenthesis(es), first at position "
           << firstOpen << "\n";
        printInputWithPointer(tokens, firstOpen, io);
        return false;
    }

    return true;
}

void SLRParser::printInputWithPointer(const std::vector<std::string>& tokens, const size_t errorPos,
                                      std::ostream& io) {
    const size_t start = (errorPos >= 3) ? (errorPos - 3) : 0;
    const size_t end   = std::min(tokens.size(), errorPos + 4);

    std::string  displayStr;
    if (start > 0)
        displayStr += "... ";

    for (size_t i = start; i < end; ++i) {
        if (i == errorPos) {
            displayStr += "--> " + tokens[i] + " <--";
        } else {
            displayStr += tokens[i];
            if (i + 1 < end)
                displayStr += " ";
        }
    }

    if (end < tokens.size())
        displayStr += " ...";

    io << "Input: " << displayStr << "\n";
}

std::unique_ptr<expr::ASTNode> SLRParser::parse(const std::vector<std::string>& inputTokens,
                                                std::ostream&                   io) const {
    std::vector<int>                            stateStack = {0};
    std::vector<std::unique_ptr<expr::ASTNode>> valueStack;
    std::vector<std::string> lexemeStack; // <-- keeps original lexemes for OP_BIN/OP_UN/FUNC1/FUNC2
    bool                     prevCanEndExpr = false; // <-- context for unary vs binary

    std::vector<std::string> input          = CFG::preprocessImplicitMul(inputTokens);
    input.emplace_back("EOS");

    if (!validateParentheses(input, io))
        return nullptr;

    size_t ptr         = 0;
    auto   productions = getProductionsVector();
    int    step        = 0;

    while (true) {
        if (step++ > 500)
            throw std::runtime_error("Parser Overflow");
        if (ptr >= input.size()) {
            io << "INTERNAL ERROR: input pointer out of range\n";
            return nullptr;
        }

        int                s          = stateStack.back();
        const std::string& tokenValue = input[ptr];
        const auto*        ops        = opConfig_;

        // -------------------------
        // 1) Map raw token -> grammar terminal (lookahead)
        // -------------------------
        std::string lookahead;

        if (tokenValue == "EOS") {
            lookahead = "EOS";
        } else if (tokenValue == "(" || tokenValue == ")" || tokenValue == ",") {
            lookahead = tokenValue;
        } else if (isNumber(tokenValue)) {
            lookahead = "NUM";
        } else if (isValidIdentifier(tokenValue)) {
            bool nextIsLParen = (ptr + 1 < input.size() && input[ptr + 1] == "(");

            if (nextIsLParen && ops && ops->isFunc2(tokenValue)) {
                lookahead = "FUNC2";
            } else if (nextIsLParen && ops && ops->isFunc1(tokenValue)) {
                lookahead = "FUNC1";
            } else {
                // allow identifier-looking operators like "f"
                if (!ops) {
                    io << "INTERNAL ERROR: OperatorConfig not set (call "
                          "parser_.setOperatorConfig)\n";
                    return nullptr;
                }

                bool canPrefix  = ops->isUnary(tokenValue);
                bool canBinary  = ops->isBinary(tokenValue);
                bool canPostfix = ops->isPostfix(tokenValue);

                if (canPrefix && canBinary) {
                    lookahead = prevCanEndExpr ? "OP_BIN" : "OP_UN_PRE";
                } else if (canPostfix) {
                    lookahead = "OP_UN_POST";
                } else if (canBinary) {
                    lookahead = "OP_BIN";
                } else if (canPrefix) {
                    lookahead = "OP_UN_PRE";
                } else {
                    std::string sug = suggestFuncCall(tokenValue, ops);
                    if (!sug.empty()) {
                        io << "Syntax error at position " << ptr << ": '" << tokenValue
                           << "' looks like a function call without parentheses. "
                           << "Did you mean " << sug << " ?\n";
                    } else {
                        io << "Syntax error at position " << ptr << ": unknown word '" << tokenValue
                           << "'\n";
                    }
                    return nullptr;
                }
            }
        } else {
            // operator symbol lexeme: decide OP_UN vs OP_BIN with context
            if (!ops) {
                io << "INTERNAL ERROR: OperatorConfig not set (call parser_.setOperatorConfig)\n";
                return nullptr;
            }

            bool nextIsLParen = (ptr + 1 < input.size() && input[ptr + 1] == "(");

            if (nextIsLParen && ops->isFunc2(tokenValue)) {
                lookahead = "FUNC2";
            } else if (nextIsLParen && ops->isFunc1(tokenValue)) {
                lookahead = "FUNC1";
            } else {
                bool canPrefix  = ops->isUnary(tokenValue);
                bool canBinary  = ops->isBinary(tokenValue);
                bool canPostfix = ops->isPostfix(tokenValue);

                if (canPrefix && canBinary) {
                    lookahead = prevCanEndExpr ? "OP_BIN" : "OP_UN_PRE";
                } else if (canPostfix) {
                    lookahead = "OP_UN_POST";
                } else if (canBinary) {
                    lookahead = "OP_BIN";
                } else if (canPrefix) {
                    lookahead = "OP_UN_PRE";
                } else {
                    io << "Syntax error at position " << ptr << ": unknown symbol '" << tokenValue
                       << "'\n";
                    return nullptr;
                }
            }
        }

        // that way users know what the error is instead of num, id, etc.
        auto pretty = [&](const std::string& t) -> std::string {
            if (t == "NUM")
                return "number";
            if (t == "OP_BIN")
                return "operator (+, -, *, /, ^, ...)";
            if (t == "OP_UN_PRE")
                return "prefix operator (-,+, ...)";
            if (t == "OP_UN_POST")
                return "postfix operator (%,..)";
            if (t == "FUNC1")
                return "function (sin, cos, ...)";
            if (t == "FUNC2")
                return "function (2 args)";
            if (t == "EOS")
                return "end of input";
            return "'" + t + "'"; // for (, ), ,
        };

        // -------------------------
        // 2) Action lookup
        // -------------------------
        if (!actionTable.contains(s) || !actionTable.at(s).contains(lookahead)) {
            std::vector<std::string> expected;
            if (actionTable.contains(s))
                for (const auto& sym : actionTable.at(s) | views::keys)
                    expected.push_back(pretty(sym));
            io << "Syntax error at position " << ptr << ": unexpected '" << tokenValue << "' (as "
               << pretty(lookahead) << ")";

            if (!expected.empty()) {
                io << ". Expected one of: ";
                for (size_t i = 0; i < expected.size(); ++i) {
                    io << expected[i];
                    if (i + 1 < expected.size())
                        io << ", ";
                }
            }

            io << "\n";
            return nullptr;
        }

        std::string act = actionTable.at(s).at(lookahead);

        // -------------------------
        // 3) ACCEPT
        // -------------------------
        if (act == "acc") {
            io << "ACCEPTED! AST created.\n";
            if (valueStack.size() != 1) {
                io << "INTERNAL ERROR: AST stack size at accept = " << valueStack.size() << "\n";
                return nullptr;
            }
            return std::move(valueStack.back());
        }

        // -------------------------
        // 4) SHIFT
        // -------------------------
        if (!act.empty() && act[0] == 's') {
            int nextState = std::stoi(act.substr(1));
            stateStack.push_back(nextState);

            // Keep original lexeme for later AST construction
            lexemeStack.push_back(tokenValue);

            // AST leaf nodes (only for NUM)
            if (lookahead == "NUM") {
                try {
                    double val = std::stod(tokenValue);
                    valueStack.push_back(std::make_unique<expr::NumberNode>(val));
                } catch (...) {
                    io << "Error: Invalid number literal '" << tokenValue << "'\n";
                    return nullptr;
                }
            } else {
                // Operators, parentheses, commas, function names: no direct AST leaf
                valueStack.push_back(nullptr);
            }

            // Update context: after these tokens we "can end an expression"
            if (lookahead == "NUM" || lookahead == ")" || lookahead == "OP_UN_POST")
                prevCanEndExpr = true;
            else
                prevCanEndExpr = false;

            ++ptr;
            continue;
        }

        // -------------------------
        // 5) REDUCE
        // -------------------------
        if (!act.empty() && act[0] == 'r') {
            int                             pIdx = std::stoi(act.substr(1));
            auto                            prod = productions[pIdx];

            const std::string&              head = prod.first;
            const std::vector<std::string>& body = prod.second;
            const size_t                    N    = body.size();

            // 1) Pop states
            for (size_t k = 0; k < N; ++k)
                stateStack.pop_back();

            // 2) Pop values
            std::vector<std::unique_ptr<expr::ASTNode>> poppedValues;
            poppedValues.reserve(N);
            for (size_t k = 0; k < N; ++k) {
                poppedValues.insert(poppedValues.begin(), std::move(valueStack.back()));
                valueStack.pop_back();
            }

            // 3) Pop lexemes
            std::vector<std::string> poppedLexemes;
            poppedLexemes.reserve(N);
            for (size_t k = 0; k < N; ++k) {
                poppedLexemes.insert(poppedLexemes.begin(), lexemeStack.back());
                lexemeStack.pop_back();
            }

            std::unique_ptr<expr::ASTNode> newNode = nullptr;

            // ---- AST building for the generic grammar ----

            // Forwarding unit productions: S->E, E->T, T->F, F->G, G->NUM, ...
            if (N == 1) {
                newNode = std::move(poppedValues[0]);
            }

            // Parentheses: G -> ( E )
            else if (N == 3 && body[0] == "(" && body[2] == ")") {
                auto inside = std::move(poppedValues[1]);
                if (!inside) {
                    io << "INTERNAL ERROR: missing expression inside parentheses\n";
                    return nullptr;
                }
                newNode = std::make_unique<expr::GroupNode>(std::move(inside));
            }
            // Prefix operator: T -> OP_UN_PRE T
            else if (N == 2 && body[0] == "OP_UN_PRE") {
                const std::string& opLex = poppedLexemes[0];
                auto               child = std::move(poppedValues[1]);
                if (!child) {
                    io << "INTERNAL ERROR: missing operand for unary op '" << opLex << "'\n";
                    return nullptr;
                }
                newNode = std::make_unique<expr::CustomUnaryOpNode>(opLex, std::move(child));
            }
            // Postfix unary : F -> F OP_UN_POST
            else if (N == 2 && body[1] == "OP_UN_POST") {
                const std::string& opLex = poppedLexemes[1];
                auto               child = std::move(poppedValues[0]);
                if (!child) {
                    io << "INTERNAL ERROR: missing operand for postfix op '" << opLex << "'\n";
                    return nullptr;
                }
                newNode = std::make_unique<expr::CustomUnaryOpNode>(opLex, std::move(child));
            }
            // Binary operator: E -> E OP_BIN T
            else if (N == 3 && body[1] == "OP_BIN") {
                auto               left  = std::move(poppedValues[0]);
                auto               right = std::move(poppedValues[2]);
                const std::string& opLex = poppedLexemes[1];

                if (!left || !right) {
                    io << "INTERNAL ERROR: missing operand for binary op '" << opLex << "'\n";
                    return nullptr;
                }
                newNode =
                    insertBinaryWithPrecedence(opLex, std::move(left), std::move(right), opConfig_);
            }
            // FUNC1: G -> FUNC1 ( E )
            else if (N == 4 && body[0] == "FUNC1" && body[1] == "(" && body[3] == ")") {
                const std::string& funcLex = poppedLexemes[0];
                auto               arg     = std::move(poppedValues[2]);
                if (!arg) {
                    io << "INTERNAL ERROR: missing argument for func1 '" << funcLex << "'\n";
                    return nullptr;
                }
                newNode = std::make_unique<expr::CustomUnaryOpNode>(funcLex, std::move(arg));
            }
            // FUNC2: G -> FUNC2 ( E , E )
            else if (N == 6 && body[0] == "FUNC2" && body[1] == "(" && body[3] == "," &&
                     body[5] == ")") {
                const std::string& funcLex = poppedLexemes[0];
                auto               a       = std::move(poppedValues[2]);
                auto               b       = std::move(poppedValues[4]);
                if (!a || !b) {
                    io << "INTERNAL ERROR: missing argument(s) for func2 '" << funcLex << "'\n";
                    return nullptr;
                }
                newNode =
                    std::make_unique<expr::CustomBinaryOpNode>(funcLex, std::move(a), std::move(b));
            } else {
                io << "INTERNAL ERROR: Unhandled production " << head << " -> ";
                for (const auto& sym : body)
                    io << sym << " ";
                io << "\n";
                return nullptr;
            }

            if (!newNode) {
                io << "INTERNAL ERROR: Failed to create AST node for production " << head << " -> ";
                for (const auto& sym : body)
                    io << sym << " ";
                io << "\n";
                return nullptr;
            }

            // Push reduced symbol + AST
            valueStack.push_back(std::move(newNode));
            lexemeStack.push_back(head); // placeholder (only length matters); safe to keep head

            prevCanEndExpr = true;

            // GOTO
            if (int top = stateStack.back();
                gotoTable.contains(top) && gotoTable.at(top).contains(head)) {
                int gotoState = gotoTable.at(top).at(head);
                stateStack.push_back(gotoState);

            } else {
                io << "INTERNAL ERROR: No GOTO for " << head << " in state " << top << "\n";
                return nullptr;
            }

            continue;
        }

        io << "INTERNAL ERROR: Unknown parser action '" << act << "'\n";
        return nullptr;
    }
}
