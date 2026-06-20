from src.parser.AST.Node import FunctionNode


class IssuePrinter:
    """Represents a compiler error or warning with location awareness."""

    def __init__(self, node, message, code, is_warning=False):
        """
        :param node: Can be an AST Node object or a SyntaxErrorNode helper.
        :param message: The description of the issue.
        :param code: A unique identifier (e.g., E101).
        :param is_warning: Boolean to toggle severity.
        """
        # We use getattr to safely extract coordinates from AST nodes
        # or fallback to 0 if the node is malformed or None.
        self.line = getattr(node, "line", 0)
        self.column = getattr(node, "column", 0)

        self.message = message
        self.code = code
        self.severity = "Warning" if is_warning else "Error"

    def __str__(self):
        """Formats the issue for terminal output."""
        code_str = f" {self.code}" if self.code else ""
        return (
            f"[ {self.severity.upper()} ]{code_str} "
            f"at {self.line}:{self.column} - {self.message}"
        )


class SyntaxErrorNode:
    """
    A lightweight wrapper used to pass line/column info to IssuePrinter
    before the AST has been built.
    """

    def __init__(self, line, column):
        self.line = line
        self.column = column


# --- Factory Functions for Semantic Issues ---


def implicit_conversion_warning(node, type_from, type_to):
    return IssuePrinter(
        node,
        f"Implicit conversion from {type_from} to {type_to}. Possible loss of data.",
        code="W001",
        is_warning=True,
    )


def literal_assigment_to_pointer_warning(node, value):
    return IssuePrinter(
        node,
        f"Literal assignment to pointer '{value}', will likely cause runtime errors",
        code="W002",
        is_warning=True,
    )


def incompatible_pointer_types_warning(node, expected, found):
    return IssuePrinter(
        node,
        f"Type mismatch: incompatible pointer types initializing '{expected}' with an expression of type '{found}'",
        code="W003",
        is_warning=True,
    )


def multi_character_char_warning(node):
    return IssuePrinter(
        node,
        "Multi-character character constant",
        code="W004",
        is_warning=True,
    )


def void_should_not_return_value_warning(node, function_name):
    return IssuePrinter(
        node,
        f"void function '{function_name}' should not return a value",
        code="W005",
        is_warning=True,
    )


def typedef_already_declared_warning(node, name):
    return IssuePrinter(
        node, f"redefinition of typedef '{name}'", code="W006", is_warning=True
    )


def implicit_declaration_warning(node, func_name):
    return IssuePrinter(
        node, f"Implicit declaration of '{func_name}'", code="W007", is_warning=True
    )


def discarded_const_qualifier_warning(node, left_type, right_type):
    return IssuePrinter(
        node,
        f"initialization discards ‘const’ qualifier from pointer target type (expected {left_type} got {right_type})",
        code="W008",
        is_warning=True,
    )


def return_type_main_not_int(node, return_type):
    return IssuePrinter(
        node,
        f"main function should return int, not {return_type}",
        code="W009",
        is_warning=True,
    )


def unknown_variable_error(node, name):
    return IssuePrinter(node, f"Use of undeclared identifier '{name}'", code="E101")


def already_declared_error(node, name):
    return IssuePrinter(node, f"Redefinition of '{name}'", code="E102")


def const_is_invariable_error(node, name):
    return IssuePrinter(
        node,
        f"Cannot assign to variable '{name}' with const-qualified type",
        code="E103",
    )


def type_mismatch_error(node, expected, found):
    return IssuePrinter(
        node, f"Type mismatch: expected {expected} but found {found}", code="E104"
    )


def missing_main_error(node):
    return IssuePrinter(
        node,
        "missing 'main' function         <-- THIS PROBABLY CAUSES OTHER ERRORS",
        code="E105",
    )


def unknown_function_error(node, func_name):
    return IssuePrinter(node, f"Unknown function '{func_name}'", code="E106")


def wrong_argument_count_error(node, func_name, expected_types, actual_arg_types):
    return IssuePrinter(
        node,
        f"Wrong argument count for '{func_name}', expected: {expected_types} but got {actual_arg_types}",
        code="E107",
    )


def dereferencing_non_pointer_error(node):
    return IssuePrinter(node, "Cannot dereference non pointer", code="E108")


def assignment_to_rvalue_error(node):
    return IssuePrinter(node, "Cannot assign to rvalue", code="E109")


def invalid_binary_operands_error(node, type1, type2):
    return IssuePrinter(
        node, f"Invalid operands for binary operator: '{type1}' '{type2}'", code="E110"
    )


def list_index_out_of_range_error(node, index):
    return IssuePrinter(node, f"List index out of range: {index}", code="E111")


def accessing_array_no_int_error(node):
    return IssuePrinter(node, "Cannot access array with non-integer index", code="E112")


def address_of_error(node):
    return IssuePrinter(node, "Cannot get address of r-value", code="E113")


def array_size_mismatch_error(node, expected, actual):
    return IssuePrinter(
        node,
        f"Array of size {expected} got initializer of length {actual}",
        code="E114",
    )


def missing_feature_error(node, feature):
    return IssuePrinter(node, f"'{feature}' is not (yet) implemented", code="E999")


def assigning_void_to_variable_error(node, variable_name):
    return IssuePrinter(
        node, f"Cannot assign void to variable '{variable_name}'", code="E115"
    )


def declaration_within_switch_error(node):
    return IssuePrinter(
        node,
        "Cannot declare within switch statement except when having made a seperate scope",
        code="E116",
    )


def undeclared_function_error(node, name):
    return IssuePrinter(node, f"Call to undefined function '{name}'", code="E117")


def break_not_in_loop_error(node):
    return IssuePrinter(node, "Break statement not in loop", code="E118")


def continue_not_in_loop_error(node):
    return IssuePrinter(node, "Continue statement not in loop", code="E119")


def global_initializer_not_constant_error(node):
    return IssuePrinter(
        node, "initializer element is not a compile-time constant", code="E120"
    )


def return_type_mismatch_error(node, return_type, returned_type):
    return IssuePrinter(
        node,
        f"Returned type '{returned_type}' cannot be automatically converted into '{return_type}'",
        code="E121",
    )


def reference_of_rvalue_error(node, type):
    return IssuePrinter(
        node, f"Cannot take address of an rvalue of type '{type}'", code="E122"
    )


def declaration_in_for_loop_error(node):
    return IssuePrinter(
        node,
        "Declarations are not allowed in for loops by default in C89.",
        code="E123",
    )


def non_existant_file_error(node, file):
    return IssuePrinter(node, f"{file}: No such file or directory.", code="E124")


def unary_not_on_pointer_error(node):
    return IssuePrinter(node, "Unary operator not allowed on pointer", code="E125")


def pasing_void_to_parameter_error(node, param_type):
    return IssuePrinter(
        node, f"Cannot pass void to parameter of type '{param_type}'", code="E126"
    )


def block_comment_within_block_comment_error(node):
    return IssuePrinter(node, "Block comments cannot be nested", code="E127")


def missing_return_error(node: FunctionNode):
    return IssuePrinter(
        node, f"Function '{node.name}' is missing a return", code="E128"
    )
