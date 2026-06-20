from src.parser.AST.Node import *
from src.parser.AST.Visitor import AstVisitor
import math


class ConstantFolder(AstVisitor):
    def __init__(self, enabled=True):
        self.enabled = enabled
        self.changed = False

    def visit_ProgramNode(self, node):
        new_headers = []
        for elem in node.header_elements:
            new_elem = yield elem
            new_headers.append(new_elem if new_elem else elem)

        return ProgramNode(
            node.line,
            node.column,
            header_elements=new_headers,
            comments=node.comments,
        )

    def visit_MainFunctionNode(self, node):
        new_statements = []
        for stmt in node.statements:
            new_stmt = yield stmt
            if new_stmt:
                new_statements.append(new_stmt)
        return MainFunctionNode(
            node.line, node.column, new_statements, comments=node.comments
        )

    def visit_IfNode(self, node):
        new_condition = yield node.condition
        new_if_block = yield node.if_block
        new_else_block = yield node.else_block if node.has_else else None

        return IfNode(
            node.line,
            node.column,
            new_condition,
            new_if_block,
            new_else_block,
            comments=node.comments,
        )

    def visit_BlockNode(self, node):
        new_statements = []
        for stmt in node.statements:
            new_stmt = yield stmt
            if new_stmt:
                new_statements.append(new_stmt)
        return BlockNode(node.line, node.column, new_statements, comments=node.comments)

    def visit_DeclarationNode(self, node):
        new_init = yield node.initializer if node.initializer else None
        return DeclarationNode(
            node.line,
            node.column,
            node.datatype,
            node.name,
            new_init,
            comments=node.comments,
        )

    def visit_AssignmentNode(self, node):
        # In the new grammar, target is an lvalue node (Variable, ArrayAccess, etc.)
        new_target = yield node.target
        new_expr = yield node.expression

        return AssignmentNode(
            node.line, node.column, new_target, new_expr, comments=node.comments
        )

    def visit_BinaryOpNode(self, node):
        left_opt = yield node.left
        right_opt = yield node.right

        if (
            self.enabled
            and isinstance(left_opt, LiteralNode)
            and isinstance(right_opt, LiteralNode)
        ):
            try:
                # ANSI C style: integer division if both are ints
                is_int_div = (
                    node.operator == "/"
                    and left_opt.datatype.base_type == BaseType.INT
                    and right_opt.datatype.base_type == BaseType.INT
                )

                if is_int_div:
                    result = int(left_opt.value / right_opt.value)
                else:
                    result = self._eval_bin(
                        left_opt.value, node.operator, right_opt.value
                    )

                if result is not None:
                    self.changed = True

                    # Determine new type based on BaseType enums
                    is_float = (
                        left_opt.datatype.base_type == BaseType.FLOAT
                        or right_opt.datatype.base_type == BaseType.FLOAT
                    )
                    new_base = BaseType.FLOAT if is_float else BaseType.INT

                    # C-style comparison results are typically ints
                    if node.operator in ["==", "!=", "<", ">", "<=", ">=", "&&", "||"]:
                        new_base = BaseType.INT
                        result = int(result)
                    elif new_base == BaseType.FLOAT:
                        import ctypes

                        result = ctypes.c_float(float(result)).value

                    return LiteralNode(
                        node.line,
                        node.column,
                        result,
                        TypeNode(node.line, node.column, new_base),
                        comments=node.comments,
                    )
            except (ZeroDivisionError, ValueError, OverflowError):
                pass

        return BinaryOpNode(
            node.line,
            node.column,
            left_opt,
            node.operator,
            right_opt,
            comments=node.comments,
        )

    def visit_UnaryOpNode(self, node):
        operand_opt = yield node.operand

        if self.enabled and isinstance(operand_opt, LiteralNode):
            try:
                result = self._eval_unary(node.operator, operand_opt.value)
                if result is not None:
                    self.changed = True
                    return LiteralNode(
                        node.line,
                        node.column,
                        result,
                        operand_opt.datatype,
                        comments=node.comments,
                    )
            except (ValueError, TypeError):
                pass

        return UnaryOpNode(
            node.line,
            node.column,
            node.operator,
            operand_opt,
            postfix=node.postfix,
            comments=node.comments,
        )

    def visit_ArrayAccessNode(self, node):
        new_array = yield node.array
        new_index = yield node.index
        return ArrayAccessNode(
            node.line, node.column, new_array, new_index, comments=node.comments
        )

    def visit_InitializerListNode(self, node):
        new_elements = []
        for el in node.elements:
            new_el = yield el
            if new_el:
                new_elements.append(new_el)
            else:
                new_elements.append(el)

        return InitializerListNode(
            node.line, node.column, new_elements, comments=node.comments
        )

    def visit_FunctionCallNode(self, node):
        new_func = yield node.function
        new_args = []
        for arg in node.arguments:
            new_val = yield arg
            new_args.append(new_val)

        new_node = FunctionCallNode(
            node.line, node.column, new_func, new_args, comments=node.comments
        )

        if hasattr(node, "mangled_name"):
            new_node.mangled_name = node.mangled_name
        if hasattr(node, "inferred_type"):
            new_node.inferred_type = node.inferred_type

        return new_node

    def visit_CastNode(self, node):
        new_expr = yield node.expression

        if self.enabled and isinstance(new_expr, LiteralNode):
            value = new_expr.value
            target = node.target_type.base_type
            if target == BaseType.INT:
                value = int(value)
            elif target == BaseType.FLOAT:
                import ctypes

                value = ctypes.c_float(float(value)).value
            elif target == BaseType.CHAR:
                value = int(value) % 256
                if value >= 128:
                    value -= 256
            self.changed = True
            return LiteralNode(node.line, node.column, value, node.target_type)

        return CastNode(
            node.line, node.column, node.target_type, new_expr, comments=node.comments
        )

    def visit_LiteralNode(self, node):
        return node

    def visit_VariableNode(self, node):
        return node

    def _eval_bin(self, left, op, right):
        ops = {
            "+": lambda a, b: a + b,
            "-": lambda a, b: a - b,
            "*": lambda a, b: a * b,
            "/": lambda a, b: a / b,
            "%": lambda a, b: a % b,
            "<<": lambda a, b: (
                a << b if isinstance(a, int) and isinstance(b, int) else math.nan
            ),
            ">>": lambda a, b: (
                a >> b if isinstance(a, int) and isinstance(b, int) else math.nan
            ),
            "&": lambda a, b: a & b,
            "|": lambda a, b: a | b,
            "^": lambda a, b: a ^ b,
            "==": lambda a, b: a == b,
            "!=": lambda a, b: a != b,
            "<": lambda a, b: a < b,
            ">": lambda a, b: a > b,
            "<=": lambda a, b: a <= b,
            ">=": lambda a, b: a >= b,
            "&&": lambda a, b: bool(a) and bool(b),
            "||": lambda a, b: bool(a) or bool(b),
        }
        handler = ops.get(op)
        return handler(left, right) if handler else None

    def _eval_unary(self, op, value):
        ops = {
            "-": lambda a: -a,
            "+": lambda a: +a,
            "!": lambda a: not a,
            "~": lambda a: ~int(a),
        }
        handler = ops.get(op)
        return handler(value) if handler else None

    def visit_FunctionNode(self, node):
        new_statements = None
        if getattr(node, "is_definition", False) and node.statements is not None:
            new_statements = []
            for stmt in node.statements:
                new_stmt = yield stmt
                if new_stmt:
                    new_statements.append(new_stmt)

        new_node = FunctionNode(
            node.line,
            node.column,
            node.name,
            node.parameters,
            new_statements,
            node.return_type,
            comments=node.comments,
        )

        if hasattr(node, "mangled_name"):
            new_node.mangled_name = node.mangled_name
        if hasattr(node, "is_definition"):
            new_node.is_definition = node.is_definition

        return new_node

    def visit_EnumNode(self, node):
        new_labels = []
        for lbl in node.labels:
            new_lbl = yield lbl
            new_labels.append(new_lbl if new_lbl else lbl)
        return EnumNode(
            node.line,
            node.column,
            node.name,
            new_labels,
            comments=node.comments,
        )

    def visit_ReturnNode(self, node):
        if node.expression:
            new_expr = yield node.expression
            return ReturnNode(node.line, node.column, new_expr, comments=node.comments)
        return ReturnNode(node.line, node.column, None, comments=node.comments)

    def visit_WhileLoopNode(self, node):
        new_condition = None
        if node.condition:
            new_condition = yield node.condition

        new_body_block = None
        if node.body_block:
            new_body_block = yield node.body_block

        return WhileLoopNode(
            node.line,
            node.column,
            new_condition,
            new_body_block,
            comments=node.comments,
        )

    def visit_BreakNode(self, node):
        return node

    def visit_ContinueNode(self, node):
        return node

    def visit_MemberAccessNode(self, node: MemberAccessNode):
        new_obj = yield node.object
        return MemberAccessNode(
            node.line,
            node.column,
            new_obj,
            node.field,
            pointer=node.pointer,
            comments=node.comments,
        )

    def visit_StructNode(self, node: StructNode):
        return node

    def visit_StructField(self, node: StructFieldNode):
        return node

    def visit_SizeOfNode(self, node):
        if not node.is_type:
            new_target = yield node.target
            return SizeOfNode(
                node.line,
                node.column,
                new_target,
                is_type=False,
                comments=node.comments,
            )
        return node
