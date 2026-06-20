from enum import Enum


class BaseType(Enum):
    VOID = 0
    CHAR = 1
    INT = 2
    FLOAT = 3
    STRUCT = 4
    UNION = 5

    def __str__(self):
        return self.name.lower()


class Node:
    def __init__(self, line, column, comments=None):
        if comments is None:
            comments = []
        self.children = []
        self.comments = comments
        self.line = line
        self.column = column
        self.inferred_type = None

    def accept(self, visitor):
        return visitor.visit(self)

    def add_child(self, node):
        if node is not None:
            self.children.append(node)


class TypeNode(Node):
    """
    Captures complex types:
    e.g., 'const int** arr[10]'
    """

    def __init__(
        self,
        line,
        column,
        base_type: BaseType,
        is_const=False,
        ptr_depth=0,
        ptr_const_quals=None,
        array_dimensions=None,
        struct_name=None,
        alias_name=None,
        comments=None,
    ):
        super().__init__(line, column, comments)
        self.base_type = base_type
        self.is_const = is_const
        self.ptr_depth = ptr_depth
        if ptr_const_quals is None:
            self.ptr_const_quals = [False] * ptr_depth
        else:
            quals = list(ptr_const_quals)
            if len(quals) < ptr_depth:
                quals.extend([False] * (ptr_depth - len(quals)))
            self.ptr_const_quals = quals[:ptr_depth]
        self.array_dimensions = array_dimensions
        self.struct_name = struct_name
        self.alias_name = alias_name

    def __str__(self):
        out = ""
        if self.is_const:
            out += "const "
        out += str(self.base_type) if self.alias_name is None else self.alias_name
        if self.struct_name:
            out += f" {self.struct_name}"
        for is_ptr_const in self.ptr_const_quals:
            out += "*"
            if is_ptr_const:
                out += " const"

        if self.array_dimensions:
            for dim in self.array_dimensions:
                if dim is None:
                    out += "[]"
                elif hasattr(dim, "value"):
                    out += f"[{dim.value}]"
                elif hasattr(dim, "name"):
                    out += f"[{dim.name}]"
                else:
                    out += "[expr]"
        return out

    def __eq__(self, value: object, /) -> bool:
        if not isinstance(value, TypeNode):
            return False

        if self.base_type != value.base_type:
            return False
        if self.ptr_depth != value.ptr_depth:
            return False
        if self.is_const != value.is_const:
            return False
        if self.struct_name != value.struct_name:
            return False
        if self.alias_name != value.alias_name:
            return False

        # Controleer array dimensies (als het arrays zijn)
        if self.array_dimensions != value.array_dimensions:
            # In een robuuste compiler zou je hier diep de elementen van de lijst vergelijken
            return False

        return True
        return super().__eq__(value)

    def is_basic(self, base_type: BaseType) -> bool:
        return self.base_type == base_type and self.ptr_depth == 0


class TypedefNode(Node):
    def __init__(self, line, column, name: str, target_type: TypeNode, comments=None):
        super().__init__(line, column, comments)
        self.name = name
        self.target_type = target_type

    def __str__(self) -> str:
        return f"typedef {self.target_type} {self.name}"


# --- Structural Nodes ---


class ProgramNode(Node):
    """The root of the AST, representing the entire file."""

    def __init__(
        self,
        line,
        column,
        header_elements=None,
        comments=None,
    ):
        super().__init__(line, column, comments)
        self.main_function = None
        if header_elements is None:
            return

        for header_element in header_elements:
            self.add_child(header_element)
            if isinstance(header_element, MainFunctionNode):
                self.main_function = header_element

    @property
    def header_elements(self):
        return self.children


class MainFunctionNode(Node):
    """Represents 'int main() { ... }'."""

    def __init__(self, line, column, statements, comments=None):
        super().__init__(line, column, comments)
        for stmt in statements:
            self.add_child(stmt)

    def __str__(self):
        return "int main()"

    @property
    def statements(self):
        return self.children


class FunctionNode(Node):
    def __init__(
        self, line, column, name, params, statements, return_type, comments=None
    ):
        super().__init__(line, column, comments)
        self.name = name
        self.mangled_name = name  # default is raw name
        self.return_type = return_type
        self.param_count = len(params)
        self.is_definition = statements is not None
        for param in params:
            self.add_child(param)
        if statements is not None:
            for stmt in statements:
                self.add_child(stmt)

    def __str__(self):
        paramstring = ""
        for param in self.parameters:
            paramstring += f"{param}, "
        paramstring = paramstring.strip(", ")
        return f"{self.return_type} {self.name}({paramstring})"

    @property
    def parameters(self):
        return self.children[: self.param_count]

    @property
    def parameter_types(self):
        param_types = [p.datatype for p in self.parameters if p]
        return param_types

    @property
    def statements(self):
        return self.children[self.param_count :]


class ParameterNode(Node):
    def __init__(self, line, column, datatype, name, comments=None):
        super().__init__(line, column, comments)
        self.datatype = datatype
        self.name = name

    def __str__(self):
        return f"{self.datatype} {self.name}"


class EnumNode(Node):
    """Represents an enum definition (enum ID { ... })."""

    def __init__(self, line, column, name, labels, comments=None):
        super().__init__(line, column, comments)
        self.name = name
        for lbl in labels:
            self.add_child(lbl)

    def __str__(self):
        return f"enum {self.name}" if self.name else "enum"

    @property
    def labels(self):
        return self.children


# --- Statement & Declaration Nodes ---


class DeclarationNode(Node):
    """Handles 'type ID' or 'type ID = expr'."""

    def __init__(
        self, line, column, datatype: TypeNode, name, initializer=None, comments=None
    ):
        super().__init__(line, column, comments)
        self.datatype = datatype
        self.name = name
        if initializer:
            self.add_child(initializer)

    def __str__(self):
        out = f"{self.datatype} {self.name}"
        if self.initializer:
            out += f" = {self.initializer}"
        return out

    @property
    def initializer(self):
        return self.children[0] if self.children else None


class AssignmentNode(Node):
    """
    Handles 'target = expr'.
    The target is now a Node (VariableNode, ArrayAccessNode, or UnaryOpNode for dereference).
    """

    def __init__(self, line, column, target, expression, comments=None):
        super().__init__(line, column, comments)
        self.add_child(target)
        self.add_child(expression)

    def __str__(self):
        return f"{self.target} = {self.expression}"

    @property
    def target(self):
        return self.children[0]

    @property
    def expression(self):
        return self.children[1]


class IncludeNode(Node):
    def __init__(self, line, column, file, comments=None):
        super().__init__(line, column, comments)
        self.file = file
        self.system = False


class BlockNode(Node):
    def __init__(self, line, column, statements, comments=None):
        super().__init__(line, column, comments)
        for stmt in statements:
            self.add_child(stmt)
        self.create_scope = True

    @property
    def statements(self):
        return self.children


class IfNode(Node):
    def __init__(
        self, line, column, condition, if_block, else_block=None, comments=None
    ):
        super().__init__(line, column, comments)
        self.has_else = else_block is not None

        self.add_child(condition)
        self.add_child(if_block)
        if self.has_else:
            self.add_child(else_block)

    def __str__(self):
        return f"if ({self.condition})"

    @property
    def condition(self):
        return self.children[0]

    @property
    def if_block(self):
        return self.children[1]

    @property
    def else_block(self):
        return self.children[2] if self.has_else else None


class WhileLoopNode(Node):
    def __init__(self, line, column, condition, body_block, comments=None):
        super().__init__(line, column, comments)
        self.add_child(condition)
        self.add_child(body_block)

    def __str__(self):
        return f"while({self.condition})"

    @property
    def condition(self):
        return self.children[0]

    @property
    def body_block(self):
        return self.children[1]

    @condition.setter
    def condition(self, value):
        self._condition = value

    @body_block.setter
    def body_block(self, value):
        self._body_block = value


# --- Expression Nodes ---


class BinaryOpNode(Node):
    """Covers all binary operations (arithmetic, logic, bitwise, comparison)."""

    def __init__(self, line, column, left, operator, right, comments=None):
        super().__init__(line, column, comments)
        self.operator = operator
        self.add_child(left)
        self.add_child(right)

    def __str__(self):
        return f"{self.left} {self.operator} {self.right}"

    @property
    def left(self):
        return self.children[0]

    @property
    def right(self):
        return self.children[1]


class UnaryOpNode(Node):
    """Covers prefix/postfix ops (++, --, !, ~, *, &, etc.)."""

    def __init__(self, line, column, operator, operand, postfix=False, comments=None):
        super().__init__(line, column, comments)
        self.operator = operator
        self.postfix = postfix
        self.add_child(operand)

    def __str__(self):
        if self.postfix:
            return f"{self.operand}{self.operator}"
        else:
            return f"{self.operator}{self.operand}"

    @property
    def operand(self):
        return self.children[0]


class ArrayAccessNode(Node):
    """Represents 'expr[expr]'."""

    def __init__(self, line, column, array, index, comments=None):
        super().__init__(line, column, comments)
        self.add_child(array)
        self.add_child(index)

    def __str__(self):
        return f"{self.array}[{self.index}]"

    @property
    def array(self):
        return self.children[0]

    @property
    def index(self):
        return self.children[1]


class FunctionCallNode(Node):
    """Represents 'expr(arg1, arg2, ...)'."""

    def __init__(self, line, column, function, arguments, comments=None):
        super().__init__(line, column, comments)
        self.mangled_name = None
        self.add_child(function)
        for arg in arguments:
            self.add_child(arg)

    def __str__(self):
        argumentstring = ""
        for arg in self.arguments:
            argumentstring += f"{arg}, "
        self.ar = argumentstring.strip(", ")
        return f"{self.function}({argumentstring})"

    @property
    def function(self):
        return self.children[0]

    @property
    def arguments(self):
        return self.children[1:]


class CastNode(Node):
    """Specifically for the '(type) expr' grammar rule."""

    def __init__(self, line, column, target_type, expression, comments=None):
        super().__init__(line, column, comments)
        self.target_type = target_type  # Expects a TypeNode
        self.add_child(expression)

    def __str__(self):
        return f"({self.target_type}) {self.expression}"

    @property
    def expression(self):
        return self.children[0]


class InitializerListNode(Node):
    """Represents an array initializer list: {expr1, expr2, ...}"""

    def __init__(self, line, column, elements, comments=None):
        super().__init__(line, column, comments)
        for element in elements:
            self.add_child(element)

    def __str__(self):
        expressionstring = ""
        for expr in self.elements:
            expressionstring += f"{expr}, "
        expressionstring = expressionstring.strip(", ")
        return f"{{{expressionstring}}}"

    @property
    def elements(self):
        return self.children


# --- Leaf Nodes ---


class LiteralNode(Node):
    """Represents INT, REAL, CHAR, STRING constants, or ENUM LABELS."""

    def __init__(
        self,
        line,
        column,
        value,
        datatype,
        enum_label=None,
        enum_name=None,
        comments=None,
    ):
        super().__init__(line, column, comments)
        self.value = value
        self.datatype = datatype  # Expects a TypeNode
        self.enum_label = enum_label
        self.enum_name = enum_name

        # If the enum label is initialized with an expression tree, store it as a child.
        if isinstance(value, Node):
            self.add_child(value)

    def __str__(self):
        if self.enum_label is not None:
            return self.enum_label

        val = self.value
        if self.datatype.base_type == BaseType.CHAR and self.datatype.ptr_depth == 0:
            char_repr = repr(chr(val)) if isinstance(val, int) else repr(val)
            char_repr = char_repr[1:-1].replace("\\", "\\\\")
            display_val = f"'{char_repr}'"
        elif self.datatype.base_type == BaseType.CHAR and self.datatype.ptr_depth > 0:
            display_val = f'"{val}"'
        else:
            display_val = str(val).replace('"', '\\"')
        return display_val


class VariableNode(Node):
    """Represents a variable usage (ID)."""

    def __init__(self, line, column, name, comments=None):
        super().__init__(line, column, comments)
        self.name = name

    def __str__(self):
        return f"{self.name}"


class ReturnNode(Node):
    def __init__(self, line, column, expression=None, comments=None):
        super().__init__(line, column, comments)
        if expression:
            self.add_child(expression)

    def __str__(self) -> str:
        return f"return {self.expression};"

    @property
    def expression(self):
        return self.children[0] if self.children else None


class BreakNode(Node):
    def __init__(self, line, column, comments=None):
        super().__init__(line, column, comments)

    def __str__(self) -> str:
        return "break;"


class ContinueNode(Node):
    def __init__(self, line, column, comments=None):
        super().__init__(line, column, comments)

    def __str__(self) -> str:
        return "continue;"


class StructFieldNode(Node):
    def __init__(self, line, column, name, datatype: TypeNode, comments=None):
        super().__init__(line, column, comments)
        self.name = name
        self.datatype = datatype

    def __str__(self) -> str:
        return f"{self.datatype} {self.name};"


class StructNode(Node):
    def __init__(
        self,
        line,
        column,
        name,
        members: list[StructFieldNode],
        is_union=False,
        comments=None,
    ):
        super().__init__(line, column, comments)
        self.name = name
        self.is_union = is_union
        for member in members:
            self.add_child(member)

    @property
    def members(self) -> list[StructFieldNode]:
        return self.children

    def __str__(self) -> str:
        keyword = "union" if self.is_union else "struct"
        return f"{keyword} {self.name}"


class MemberAccessNode(Node):
    def __init__(
        self,
        line,
        column,
        object: Node,
        field: str,
        pointer: bool = False,
        comments=None,
    ):
        super().__init__(line, column, comments)
        self.object = object
        self.field = field
        self.pointer = pointer

    def __str__(self) -> str:
        out = f"{self.object}"
        out += "->" if self.pointer else "."
        out += self.field
        return out


class SizeOfNode(Node):
    def __init__(self, line, column, target, is_type=False, comments=None):
        super().__init__(line, column, comments)
        self.target = target
        self.is_type = is_type
        if not is_type and target is not None:
            self.add_child(target)

    def __str__(self):
        return f"sizeof({self.target})"
