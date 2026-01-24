#include "../PDA.h"

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

// Test 1: Simple number
static void test_simple_number(PDA& pda) {
    bool accepted = pda.simulate("5");
    assert(accepted && "PDA should accept '5'");
    auto steps = pda.getLastSimulation();
    assert(!steps.empty() && "Simulation should have steps");
    assert(pda.wasLastSimulationAccepted() && "Simulation should be marked as accepted");
}

// Test 2: Simple addition
static void test_simple_addition(PDA& pda) {
    bool accepted = pda.simulate("5+3");
    assert(accepted && "PDA should accept '5+3'");
    auto steps = pda.getLastSimulation();
    assert(!steps.empty() && "Simulation should have steps");
}

// Test 3: Multiplication
static void test_multiplication(PDA& pda) {
    bool accepted = pda.simulate("2*3");
    assert(accepted && "PDA should accept '2*3'");
}

// Test 4: Complex expression
static void test_complex_expression(PDA& pda) {
    bool accepted = pda.simulate("5+3*2");
    assert(accepted && "PDA should accept '5+3*2'");
}

// Test 5: Parentheses
static void test_parentheses(PDA& pda) {
    bool accepted = pda.simulate("(5+3)*2");
    assert(accepted && "PDA should accept '(5+3)*2'");
}

// Test 6: Identifier
static void test_identifier(PDA& pda) {
    bool accepted = pda.simulate("x+5");
    assert(!accepted && "PDA should reject 'x+5'");
}

// Test 7: Unary plus is valid
static void test_unary_plus(PDA& pda) {
    bool accepted = pda.simulate("5++3");
    assert(accepted && "PDA should accept '5++3' as 5 + (+3)");
}

// Test 8: Invalid - trailing operator
static void test_invalid_trailing_operator(PDA& pda) {
    bool accepted = pda.simulate("5+");
    assert(!accepted && "PDA should reject '5+'");
}

// Test 9: Unary operator
static void test_unary_operator(PDA& pda) {
    bool accepted = pda.simulate("-5");
    assert(accepted && "PDA should accept '-5'");
}

// Test 10: Nested parentheses
static void test_nested_parentheses(PDA& pda) {
    bool accepted = pda.simulate("((5+3)*2)");
    assert(accepted && "PDA should accept '((5+3)*2)'");
}

// Test 11: Division
static void test_division(PDA& pda) {
    bool accepted = pda.simulate("8/2");
    assert(accepted && "PDA should accept '8/2'");
}

// Test 12: Subtraction
static void test_subtraction(PDA& pda) {
    bool accepted = pda.simulate("10-3");
    assert(accepted && "PDA should accept '10-3'");
}

// Test 13: Verify simulation steps structure
static void test_simulation_steps_structure(PDA& pda) {
    pda.simulate("5+3");
    auto steps = pda.getLastSimulation();

    assert(!steps.empty() && "Steps should not be empty");

    // Check first step
    assert(!steps[0].currentState.empty() && "Step should have current state");
    assert(!steps[0].action.empty() && "Step should have action");
    assert(steps[0].stepNumber > 0 && "Step number should be positive");

    // Check last step should be accept
    assert(steps.back().action == "accept" && "Last step should be accept");
}

// Test 14: Verify error message for invalid input
static void test_error_message_quality(PDA& pda) {
    pda.simulate("5++");
    std::string errorMsg = pda.getErrorMessage();

    assert(!errorMsg.empty() && "Error message should not be empty for invalid input");
    assert((errorMsg.find("Rejected") != std::string::npos ||
            errorMsg.find("rejected") != std::string::npos) &&
           "Error message should mention rejection");
}

// Test 15: Multi-digit numbers
static void test_multi_digit_numbers(PDA& pda) {
    bool accepted = pda.simulate("123+456");
    assert(accepted && "PDA should accept '123+456'");
}

// Test 16: Multiple operators
static void test_multiple_operators(PDA& pda) {
    bool accepted = pda.simulate("2+3*4-5/2");
    assert(accepted && "PDA should accept '2+3*4-5/2'");
}

// Test 17: Deeply nested parentheses
static void test_deeply_nested_parentheses(PDA& pda) {
    bool accepted = pda.simulate("(((5)))");
    assert(accepted && "PDA should accept '(((5)))'");
}

// Test 18: Mixed identifiers and numbers (identifiers not supported)
static void test_mixed_identifiers_and_numbers(PDA& pda) {
    bool accepted = pda.simulate("x+y*5");
    assert(!accepted && "PDA should reject 'x+y*5' because identifiers are not supported");
    std::string errorMsg = pda.getErrorMessage();
    assert(!errorMsg.empty() && "Error message should not be empty");
}

// Test 19: Unary minus with parentheses.
static void test_unary_minus_with_parentheses(PDA& pda) {
    bool accepted = pda.simulate("-(5+3)");
    assert(accepted && "PDA should accept '-(5+3)'");
}

// Test 20: Just opening parenthesis.
static void test_just_opening_parenthesis(PDA& pda) {
    bool accepted = pda.simulate("(");
    assert(!accepted && "PDA should reject '(' (incomplete expression)");
}

int main() {
    std::cout << "Running PDA tests...\n\n";

    try {
        PDA pda("../config/pda_config.json");

        std::cout << "Test 1: Simple number... ";
        test_simple_number(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 2: Simple addition... ";
        test_simple_addition(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 3: Multiplication... ";
        test_multiplication(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 4: Complex expression... ";
        test_complex_expression(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 5: Parentheses... ";
        test_parentheses(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 6: Identifier... ";
        test_identifier(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 7: Invalid double operator... ";
        test_unary_plus(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 8: Invalid trailing operator... ";
        test_invalid_trailing_operator(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 9: Unary operator... ";
        test_unary_operator(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 10: Nested parentheses... ";
        test_nested_parentheses(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 11: Division... ";
        test_division(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 12: Subtraction... ";
        test_subtraction(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 13: Simulation steps structure... ";
        test_simulation_steps_structure(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 14: Error message quality... ";
        test_error_message_quality(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 15: Multi-digit numbers... ";
        test_multi_digit_numbers(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 16: Multiple operators... ";
        test_multiple_operators(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 17: Deeply nested parentheses... ";
        test_deeply_nested_parentheses(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 18: Mixed identifiers and numbers... ";
        test_mixed_identifiers_and_numbers(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 19: Unary minus with parentheses... ";
        test_unary_minus_with_parentheses(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "Test 20: Just opening parenthesis... ";
        test_just_opening_parenthesis(pda);
        std::cout << "✓ PASSED\n";

        std::cout << "\n" << std::string(40, '=') << "\n";
        std::cout << "All 20 PDA tests passed successfully!\n";
        std::cout << std::string(40, '=') << "\n\n";

        return 0;

    } catch (const std::exception& e) {
        std::cerr << "\n✗ Test failed with exception: " << e.what() << "\n";
        return 1;
    }
}
