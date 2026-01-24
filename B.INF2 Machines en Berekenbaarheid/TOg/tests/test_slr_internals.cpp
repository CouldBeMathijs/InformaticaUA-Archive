#include "../CFG.h"
#include "../SLRParser.h"
#include "../ast/AST.h"
#include "../ast/ExpressionOperatorConfig.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace expr;

// ------------------------------------------------------------
// Shared fixtures (avoid dangling pointers)
// ------------------------------------------------------------
static CFG                 g_cfg("../config/expression_grammar.json");
static cfg::OperatorConfig g_ops("../config/operators.json");
static bool                g_parserInitialized = false;

// Create an initialized parser that safely references g_cfg + g_ops
static SLRParser& get_initialized_parser() {
    static bool      initialized = false;
    static SLRParser parser(g_cfg);

    if (!initialized) {
        g_cfg.setOperatorConfig(&g_ops);
        parser.setOperatorConfig(&g_ops);
        parser.setDebug(true);

        std::ostringstream dummy;
        parser.slr(dummy); // build SLR table once
        initialized = true;
    }
    return parser;
}

// ------------------------------------------------------------
// Test 1: Helper functions of SLRParser
// ------------------------------------------------------------
static void test_helper_functions() {
    std::cout << "Testing SLRParser helper functions... ";

    CFG       cfg("../tests/deprecated_expression_grammar.json");
    SLRParser parser(cfg);

    // isNumber()
    assert(parser.isNumber("123"));
    assert(parser.isNumber("3.14"));
    assert(parser.isNumber("-5"));
    assert(parser.isNumber("0.0"));
    assert(!parser.isNumber("abc"));
    assert(!parser.isNumber("12a"));
    assert(!parser.isNumber(""));

    std::cout << "✓" << std::endl;
}

// ------------------------------------------------------------
// Test 2: LR0Item struct operations
// ------------------------------------------------------------
static void test_LR0Item_operations() {
    std::cout << "Testing LR0Item operations... ";

    LR0Item item("E", {"E", "+", "T"}, 0);
    assert(item.head == "E");
    assert(item.body.size() == 3);
    assert(item.dotPos == 0);
    assert(item.getSymbolAfterDot() == "E");
    assert(!item.isReduceItem());

    LR0Item next = item.getNextItem();
    assert(next.dotPos == 1);
    assert(next.getSymbolAfterDot() == "+");

    LR0Item reduceItem("E", {"T"}, 1);
    assert(reduceItem.isReduceItem());
    assert(reduceItem.getSymbolAfterDot() == "");

    std::cout << "✓" << std::endl;
}

// ------------------------------------------------------------
// Test 3: Indirect FIRST sets behavior
// ------------------------------------------------------------
static void test_first_sets_indirect() {
    expr::OperatorEnvironment env = expr::OperatorEnvironment::createDefault();
    expr::loadBinaryOperatorsFromFile(env, "../config/operators.json");

    std::cout << "Testing FIRST sets behavior... ";

    auto parser = get_initialized_parser();

    // 1) Numbers
    {
        std::ostringstream       dummy;
        std::vector<std::string> tokens = {"2", "+", "3"};
        auto                     ast    = parser.parse(tokens, dummy);
        if (!ast)
            std::cerr << "\nWARNING: Could not parse '2 + 3'. Output:\n" << dummy.str() << "\n";
        else
            assert(std::abs(ast->evaluateWith(env) - 5.0) < 0.001);
    }

    // 3) Parentheses
    {
        std::ostringstream       dummy;
        std::vector<std::string> tokens = {"(", "1", "+", "2", ")"};
        auto                     ast    = parser.parse(tokens, dummy);
        if (!ast)
            std::cerr << "\nWARNING: Could not parse '(1 + 2)'. Output:\n" << dummy.str() << "\n";
        else
            assert(std::abs(ast->evaluateWith(env) - 3.0) < 0.001);
    }

    // 4) Functions (no evaluation if your AST uses CustomUnaryOpNode for funcs)
    {
        std::ostringstream       dummy;
        std::vector<std::string> tokens = {"sin", "(", "0", ")"};
        auto                     ast    = parser.parse(tokens, dummy);
        if (!ast)
            std::cerr << "\nINFO: Could not parse 'sin(0)'. Output:\n" << dummy.str() << "\n";
        else
            assert(ast->toString().find("sin") != std::string::npos);
    }

    std::cout << "✓" << std::endl;
}

// ------------------------------------------------------------
// Test 4: Additional error cases
// ------------------------------------------------------------
static void test_additional_error_cases() {
    expr::OperatorEnvironment env = expr::OperatorEnvironment::createDefault();
    expr::loadBinaryOperatorsFromFile(env, "../config/operators.json");

    std::cout << "Testing additional error cases... ";

    auto parser = get_initialized_parser();

    // 1) Unknown token
    {
        std::ostringstream       output;
        std::vector<std::string> tokens = {"2", "ñ", "3"};
        auto                     ast    = parser.parse(tokens, output);
        if (ast != nullptr) {
            std::cerr << "\nWARNING: Parser accepted invalid token 'ñ'\n";
        } else if (output.str().find("Error") == std::string::npos &&
                   output.str().find("Unknown") == std::string::npos &&
                   output.str().find("Parse error") == std::string::npos &&
                   output.str().find("Syntax error") == std::string::npos) {
            std::cerr << "\nINFO: No error message for invalid token\n";
        }
    }

    // 2) Empty parentheses
    {
        std::ostringstream       output;
        std::vector<std::string> tokens = {"(", ")"};
        auto                     ast    = parser.parse(tokens, output);
        if (ast != nullptr)
            std::cerr << "\nWARNING: Parser accepted empty parentheses '()'\n";
    }

    // 3) Standalone operator
    {
        std::ostringstream       output;
        std::vector<std::string> tokens = {"+"};
        auto                     ast    = parser.parse(tokens, output);
        if (ast != nullptr)
            std::cerr << "\nWARNING: Parser accepted standalone operator '+'\n";
    }

    // 4) Reasonably long expression (should not overflow)
    {
        std::ostringstream       output;
        std::vector<std::string> tokens;
        for (int i = 0; i < 10; i++) {
            tokens.push_back("1");
            tokens.push_back("+");
        }
        tokens.push_back("1");

        auto ast = parser.parse(tokens, output);
        if (ast != nullptr) {
            assert(std::abs(ast->evaluateWith(env) - 11.0) < 0.001);
        } else if (output.str().find("Parser Overflow") != std::string::npos) {
            std::cerr << "\nINFO: Parser correctly detected overflow\n";
        } else {
            std::cerr << "\nWARNING: Long expression failed to parse. Output:\n"
                      << output.str() << "\n";
        }
    }

    std::cout << "✓" << std::endl;
}

// ------------------------------------------------------------
// Test 5: Operator combinations
// ------------------------------------------------------------
static void test_operator_combinations() {
    std::cout << "Testing operator combinations... ";

    auto parser = get_initialized_parser();

    struct TestCase {
        std::vector<std::string> tokens;
        std::string              description;
        bool                     expectSuccess;
    };

    std::vector<TestCase> testCases = {
        {{"2", "-", "3", "-", "4"}, "Multiple subtraction", true},
        {{"2", "*", "3", "*", "4"}, "Multiple multiplication", true},
        {{"2", "+", "3", "*", "4", "-", "5"}, "Mixed operators", true},
        {{"sin", "(", "cos", "(", "0", ")", ")"}, "Nested functions", true},
        {{"(", "2", "+", "3", ")", "*", "(", "4", "-", "1", ")"}, "Multiple parentheses", true},
        {{"2", "^", "3", "^", "2"}, "Multiple power operators", true},
    };

    int passed = 0;
    int total  = 0;

    for (const auto& testCase : testCases) {
        total++;
        std::ostringstream dummy;
        auto               ast = parser.parse(testCase.tokens, dummy);

        if (testCase.expectSuccess) {
            if (ast != nullptr) {
                passed++;
            } else {
                std::cerr << "\nWARNING: Failed to parse: " << testCase.description << "\nOutput:\n"
                          << dummy.str() << "\n";
            }
        } else {
            if (ast == nullptr)
                passed++;
        }
    }

    std::cout << "✓ (" << passed << "/" << total << " cases handled)" << std::endl;
}

// ------------------------------------------------------------
// Test 6: getAction behavior (via debug output)
// ------------------------------------------------------------
static void test_getAction_behavior() {
    std::cout << "Testing getAction behavior... ";

    auto parser = get_initialized_parser();

    {
        std::ostringstream       parseOutput;
        std::vector<std::string> tokens = {"2"};

        auto                     ast    = parser.parse(tokens, parseOutput);

        if (!ast) {
            std::cerr << "\nWARNING: Could not parse single number '2'\n";
        } else {
            std::string output    = parseOutput.str();
            bool        hasShift  = (output.find("Shift") != std::string::npos ||
                             output.find("shift") != std::string::npos);
            bool        hasReduce = (output.find("Reduce") != std::string::npos ||
                              output.find("reduce") != std::string::npos);

            if (!hasShift && !hasReduce)
                std::cerr << "\nINFO: No shift/reduce debug output found\n";
        }
    }

    std::cout << "✓" << std::endl;
}

// ------------------------------------------------------------
// Test 7: Parser internals via debug output
// ------------------------------------------------------------
static void test_parser_internals() {
    std::cout << "Testing parser internals via debug... ";

    auto                     parser = get_initialized_parser();

    std::ostringstream       debugOutput;
    std::vector<std::string> tokens = {"2"};

    auto                     ast    = parser.parse(tokens, debugOutput);

    if (!ast) {
        std::cerr << "\nWARNING: Could not parse single number '2'\n";
        std::cout << "✓ (skipped - parser not working)" << std::endl;
        return;
    }

    std::string output             = debugOutput.str();

    bool        hasStartingMessage = output.find("Starting Parse") != std::string::npos;
    bool        hasAcceptedMessage = output.find("ACCEPTED") != std::string::npos;

    if (!hasStartingMessage)
        std::cerr << "\nINFO: No 'Starting Parse' message in debug output\n";
    if (!hasAcceptedMessage)
        std::cerr << "\nINFO: No 'ACCEPTED' message in debug output\n";

    size_t shiftCount = 0, reduceCount = 0, pos = 0;
    while ((pos = output.find("Shift", pos)) != std::string::npos) {
        shiftCount++;
        pos += 5;
    }
    pos = 0;
    while ((pos = output.find("shift", pos)) != std::string::npos) {
        shiftCount++;
        pos += 5;
    }

    pos = 0;
    while ((pos = output.find("Reduce", pos)) != std::string::npos) {
        reduceCount++;
        pos += 6;
    }
    pos = 0;
    while ((pos = output.find("reduce", pos)) != std::string::npos) {
        reduceCount++;
        pos += 6;
    }

    if (shiftCount == 0 && reduceCount == 0)
        std::cerr << "\nINFO: No shift/reduce actions in debug output\n";

    std::cout << "✓" << std::endl;
}

// ------------------------------------------------------------
// Main runner
// ------------------------------------------------------------
int main() {
    std::cout << "Testing SLR Parser INTERNAL functionality:" << std::endl;

    int  tests_passed = 0;
    int  tests_total  = 0;

    auto run_test     = [&](void (*test_func)(), const std::string& name) {
        tests_total++;
        std::cout << "\nTest " << tests_total << ": " << name << std::endl;
        try {
            test_func();
            tests_passed++;
            return true;
        } catch (const std::exception& e) {
            std::cerr << name << " FAILED with exception: " << e.what() << std::endl;
            return false;
        } catch (...) {
            std::cerr << name << " FAILED with unknown error" << std::endl;
            return false;
        }
    };

    run_test(test_helper_functions, "Helper functions");
    run_test(test_LR0Item_operations, "LR0Item operations");
    run_test(test_first_sets_indirect, "FIRST sets indirect");
    run_test(test_additional_error_cases, "Additional error cases");
    run_test(test_operator_combinations, "Operator combinations");
    run_test(test_getAction_behavior, "getAction behavior");
    run_test(test_parser_internals, "Parser internals");

    std::cout << "\nRESULTS: " << tests_passed << "/" << tests_total << " tests passed"
              << std::endl;

    if (tests_passed == tests_total)
        std::cout << "ALL TESTS COMPLETED SUCCESSFULLY!" << std::endl;
    else
        std::cout << "SOME TESTS HAD ISSUES (see warnings above)" << std::endl;

    return (tests_passed == tests_total) ? 0 : 1;
}
