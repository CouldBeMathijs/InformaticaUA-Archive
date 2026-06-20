from src.parser.AST.Node import *
from src.parser.AST.Visitor import AstVisitor


class ConstantPropagator(AstVisitor):
    def __init__(self, issue_collector=None):
        self.issues = issue_collector if issue_collector is not None else []
        self.constant_scopes = []
        self.type_scopes = []
        self.changed = False
        self.in_conditional = 0

    def _enter_scope(self):
        self.constant_scopes.append({})
        self.type_scopes.append({})

    def _exit_scope(self):
        if self.constant_scopes:
            self.constant_scopes.pop()
            self.type_scopes.pop()

    def _declare_variable(self, name, type_node, value=None):
        if not self.constant_scopes:
            self._enter_scope()
        self.type_scopes[-1][name] = type_node
        self.constant_scopes[-1][name] = value

    def _lookup_constant(self, name, default=None):
        for scope in reversed(self.constant_scopes):
            if name in scope:
                return scope[name]
        return default

    def _get_type(self, name):
        for scope in reversed(self.type_scopes):
            if name in scope:
                return scope[name]
        return None

    def _assign_constant(self, name, value):
        for scope in reversed(self.constant_scopes):
            if name in scope:
                if self.in_conditional > 0:
                    scope[name] = None
                else:
                    scope[name] = value
                return
        self._declare_variable(name, None, value)

    def _cast_literal(self, literal: LiteralNode, target_type: TypeNode) -> LiteralNode:
        if not isinstance(literal, LiteralNode) or target_type is None:
            return literal

        val = literal.value
        base = target_type.base_type

        if target_type.ptr_depth == 0 and not target_type.array_dimensions:
            try:
                if base == BaseType.INT:
                    val = int(val)
                elif base == BaseType.FLOAT:
                    import ctypes

                    val = ctypes.c_float(float(val)).value
                elif base == BaseType.CHAR:
                    val = int(val) % 256
                    if val >= 128:
                        val -= 256
            except (ValueError, TypeError):
                pass

        return LiteralNode(
            literal.line, literal.column, val, target_type, comments=literal.comments
        )

    # --- Structural Nodes ---

    def visit_ProgramNode(self, node: ProgramNode):
        new_headers = []
        for elem in node.header_elements:
            new_elem = yield elem
            new_headers.append(new_elem if new_elem else elem)

        return ProgramNode(
            line=node.line,
            column=node.column,
            header_elements=new_headers,
            comments=node.comments,
        )

    def visit_MainFunctionNode(self, node: MainFunctionNode):
        self.constant_scopes = []
        self.type_scopes = []
        self._enter_scope()
        new_statements = []
        for stmt in node.statements:
            new_stmt = yield stmt
            if new_stmt:
                new_statements.append(new_stmt)
        self._exit_scope()
        return MainFunctionNode(
            node.line, node.column, new_statements, comments=node.comments
        )

    def visit_DeclarationNode(self, node: DeclarationNode):
        new_init = yield node.initializer if node.initializer else None

        propagated_value = None
        if isinstance(new_init, LiteralNode):
            propagated_value = self._cast_literal(new_init, node.datatype)

        self._declare_variable(node.name, node.datatype, propagated_value)

        return DeclarationNode(
            node.line,
            node.column,
            node.datatype,
            node.name,
            new_init,
            comments=node.comments,
        )

    def visit_AssignmentNode(self, node: AssignmentNode):
        new_expr = yield node.expression

        if isinstance(node.target, VariableNode):
            target_type = self._get_type(node.target.name)

            is_literal = isinstance(new_expr, LiteralNode)
            current_const = self._lookup_constant(node.target.name, None)
            exists = self._lookup_constant(node.target.name, 1) is not None

            if is_literal and not isinstance(current_const, LiteralNode) and exists:
                casted_expr = self._cast_literal(new_expr, target_type)
                self._assign_constant(node.target.name, casted_expr)
            else:
                self._assign_constant(node.target.name, None)

        return AssignmentNode(
            node.line, node.column, node.target, new_expr, comments=node.comments
        )

    def visit_BlockNode(self, node: BlockNode):
        self._enter_scope()
        new_statements = []
        for stmt in node.statements:
            new_stmt = yield stmt
            if new_stmt:
                new_statements.append(new_stmt)
        self._exit_scope()
        return BlockNode(node.line, node.column, new_statements, comments=node.comments)

    def visit_IfNode(self, node: IfNode):
        new_condition = yield node.condition

        self.in_conditional += 1
        new_if_block = yield node.if_block
        new_else_block = yield node.else_block if node.has_else else None
        self.in_conditional -= 1

        if isinstance(new_condition, LiteralNode):
            self.changed = True
            if new_condition.value != 0:
                return new_if_block
            else:
                return new_else_block

        return IfNode(
            node.line,
            node.column,
            new_condition,
            new_if_block,
            new_else_block,
            comments=node.comments,
        )

    # --- Expression Nodes ---

    def visit_BinaryOpNode(self, node: BinaryOpNode):
        new_left = yield node.left
        new_right = yield node.right
        return BinaryOpNode(
            node.line,
            node.column,
            new_left,
            node.operator,
            new_right,
            comments=node.comments,
        )

    def visit_UnaryOpNode(self, node: UnaryOpNode):
        if node.operator == "&":
            if isinstance(node.operand, VariableNode):
                self._assign_constant(node.operand.name, None)
            return node

        if node.operator == "*":
            return node

        if node.operator in ("++", "--"):
            if isinstance(node.operand, VariableNode):
                self._assign_constant(node.operand.name, None)
            return node

        new_operand = yield node.operand
        return UnaryOpNode(
            node.line,
            node.column,
            node.operator,
            new_operand,
            postfix=node.postfix,
            comments=node.comments,
        )

    def visit_ArrayAccessNode(self, node: ArrayAccessNode):
        new_array = yield node.array
        new_index = yield node.index
        return ArrayAccessNode(
            node.line, node.column, new_array, new_index, comments=node.comments
        )

    def visit_InitializerListNode(self, node: InitializerListNode):
        new_elements = []
        for el in node.elements:
            new_el = yield el
            new_elements.append(new_el if new_el else el)

        return InitializerListNode(
            node.line, node.column, new_elements, comments=node.comments
        )

    def visit_FunctionCallNode(self, node: FunctionCallNode):
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

    def visit_CastNode(self, node: CastNode):
        new_expr = yield node.expression
        return CastNode(
            node.line, node.column, node.target_type, new_expr, comments=node.comments
        )

    # --- Leaf Nodes ---

    def visit_VariableNode(self, node: VariableNode):
        if node.inferred_type and node.inferred_type.ptr_depth > 0:
            return node

        if hasattr(node, "enum_value"):
            self.changed = True
            return LiteralNode(
                node.line,
                node.column,
                int(node.enum_value),
                TypeNode(node.line, node.column, BaseType.INT),
                comments=node.comments,
            )

        sentinel = object()
        const_node = self._lookup_constant(node.name, sentinel)
        if const_node is not sentinel:
            if const_node is None:
                return node
            self.changed = True
            return LiteralNode(
                node.line,
                node.column,
                const_node.value,
                const_node.datatype,
                comments=node.comments,
            )
        return node

    def visit_LiteralNode(self, node: LiteralNode):
        return node

    def visit_FunctionNode(self, node: FunctionNode):
        self.constant_scopes = []
        self.type_scopes = []
        self._enter_scope()

        new_statements = None
        if getattr(node, "is_definition", False) and node.statements is not None:
            new_statements = []
            for stmt in node.statements:
                new_stmt = yield stmt
                if new_stmt:
                    new_statements.append(new_stmt)
        self._exit_scope()

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

    def visit_EnumNode(self, node: EnumNode):
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

    def visit_ReturnNode(self, node: ReturnNode):
        if node.expression:
            new_expr = yield node.expression
            return ReturnNode(node.line, node.column, new_expr, comments=node.comments)
        return ReturnNode(node.line, node.column, None, comments=node.comments)

    def visit_WhileLoopNode(self, node: WhileLoopNode):
        for scope in self.constant_scopes:
            for key in scope:
                scope[key] = None

        self.in_conditional += 1
        self._enter_scope()

        new_condition = yield node.condition if node.condition else None
        new_body_block = yield node.body_block if node.body_block else None

        self._exit_scope()
        self.in_conditional -= 1

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

    def visit_StructNode(self, node: StructNode):
        return node

    def visit_StructField(self, node: StructFieldNode):
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
