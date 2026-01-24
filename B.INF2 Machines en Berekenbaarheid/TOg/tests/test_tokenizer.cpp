#include "../config/OperatorConfig.h"
#include "../utils/Tokenizer.h"

#include <cassert>
#include <iostream>
#include <vector>

using namespace Tokenize;

// test voor basis expression
void test_basic_tokens() {
    std::cout << "Testing basic tokens... ";
    cfg::OperatorConfig      ops("../config/operators.json");
    std::string              input  = "3 + 45.5 * 10";

    std::vector<std::string> tokens = tokenizeToCykSymbols(input, ops);

    assert(!tokens.empty());
    assert(tokens[0] == "NUM");
    assert(tokens[1] == "OP_BIN");
    assert(tokens[2] == "NUM");
    std::cout << "v" << std::endl;
}

void test_parentheses_and_functions() {
    std::cout << "Testing parentheses and functions... ";
    cfg::OperatorConfig      ops("../config/operators.json");
    std::string              input  = "sin(3.14)";

    std::vector<std::string> tokens = tokenizeToCykSymbols(input, ops);

    assert(tokens[0] == "FUNC1" || tokens[0] == "ID");
    assert(tokens[1] == "(");
    std::cout << "v" << std::endl;
}

void test_variables_and_identifiers() {
    std::cout << "Testing variables (ID)... ";
    cfg::OperatorConfig      ops("../config/operators.json");
    std::string              input  = "x * y + var_1";

    std::vector<std::string> tokens = tokenizeToCykSymbols(input, ops);

    // x, y en var_1 moeten als ID herkend worden
    assert(tokens.size() == 5);
    assert(tokens[0] == "ID");     // x
    assert(tokens[1] == "OP_BIN"); // *
    assert(tokens[2] == "ID");     // y
    assert(tokens[4] == "ID");     // var_1
    std::cout << "v" << std::endl;
}

void test_complex_spacing() {
    std::cout << "Testing whitespace independence... ";
    cfg::OperatorConfig ops("../config/operators.json");
    // Test input met veel en weinig spaties
    std::string              input  = "  3+   ( 4 *5)";

    std::vector<std::string> tokens = tokenizeToCykSymbols(input, ops);

    // Verwacht: NUM, OP_BIN, (, NUM, OP_BIN, NUM, )
    assert(tokens.size() == 7);
    assert(tokens[0] == "NUM");
    assert(tokens[1] == "OP_BIN");
    assert(tokens[2] == "(");
    assert(tokens[6] == ")");
    std::cout << "v" << std::endl;
}

void test_unary_minus_detection() {
    std::cout << "Testing unary vs binary minus... ";
    cfg::OperatorConfig ops("../config/operators.json");

    // In "5 - -3", de eerste is binair, de tweede is unair (teken)
    std::string              input  = "5 - -3";
    std::vector<std::string> tokens = tokenizeToCykSymbols(input, ops);

    assert(tokens[0] == "NUM");                               // 5
    assert(tokens[1] == "OP_BIN");                            // -
    assert(tokens[2] == "OP_UN_PRE" || tokens[2] == "OP_UN"); // De tweede -
    assert(tokens[3] == "NUM");                               // 3
    std::cout << "v" << std::endl;
}

int main() {
    std::cout << "RUNNING TOKENIZER TESTS" << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    try {
        test_basic_tokens();
        test_parentheses_and_functions();
        test_variables_and_identifiers();
        test_complex_spacing();
        test_unary_minus_detection();

        std::cout << "--------------------------------------" << std::endl;
        std::cout << "ALL TOKENIZER TESTS PASSED!" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "TEST FAILED: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
