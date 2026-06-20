from src.parser.AST.Node import *
from src.parser.AST.Visitor import AstVisitor


def has_side_effects(node):
    if node is None:
        return False
    if isinstance(node, (FunctionCallNode, AssignmentNode)):
        return True
    if isinstance(node, UnaryOpNode) and node.operator in ("++", "--"):
        return True
    for child in node.children:
        if has_side_effects(child):
            return True
    return False


class UsageScope:
    def __init__(self, parent=None):
        self.parent = parent
        self.symbols = {}
        self.uses = {}
        self.assignments = {}

    def define(self, name, node):
        self.symbols[name] = node
        self.uses[name] = 0
        self.assignments[name] = []

    def record_use(self, name):
        curr = self
        while curr is not None:
            if name in curr.symbols:
                curr.uses[name] += 1
                return
            curr = curr.parent


class VariableUsageAnalyzer(AstVisitor):
    def __init__(self):
        self.current_scope: UsageScope = UsageScope()
        self.unused_decls = set()
        self.dead_assignments = set()

    def _enter_scope(self):
        self.current_scope = UsageScope(parent=self.current_scope)

    def _exit_scope(self):
        for name, count in self.current_scope.uses.items():
            if count == 0:
                decl_node = self.current_scope.symbols.get(name)
                if isinstance(decl_node, DeclarationNode):
                    self.unused_decls.add(id(decl_node))
                    for assign_node in self.current_scope.assignments.get(name, []):
                        self.dead_assignments.add(id(assign_node))
        self.current_scope = self.current_scope.parent

    def visit_ProgramNode(self, node: ProgramNode):
        self._enter_scope()
        for child in node.children:
            yield child
        self._exit_scope()
        return node

    def visit_MainFunctionNode(self, node: MainFunctionNode):
        self._enter_scope()
        if hasattr(node, "statements") and node.statements:
            for stmt in node.statements:
                yield stmt
        self._exit_scope()
        return node

    def visit_FunctionNode(self, node: FunctionNode):
        self._enter_scope()
        for param in node.parameters:
            self.current_scope.define(param.name, param)
        if getattr(node, "statements", None):
            for stmt in node.statements:
                yield stmt
        self._exit_scope()
        return node

    def visit_BlockNode(self, node: BlockNode):
        self._enter_scope()
        if hasattr(node, "statements") and node.statements:
            for stmt in node.statements:
                yield stmt
        self._exit_scope()
        return node

    def visit_IfNode(self, node: IfNode):
        yield node.condition
        self._enter_scope()
        yield node.if_block
        self._exit_scope()
        if getattr(node, "has_else", False) and node.else_block:
            self._enter_scope()
            yield node.else_block
            self._exit_scope()
        return node

    def visit_WhileLoopNode(self, node: WhileLoopNode):
        self._enter_scope()
        if node.condition:
            yield node.condition
        if node.body_block:
            yield node.body_block
        self._exit_scope()
        return node

    def visit_DeclarationNode(self, node: DeclarationNode):
        if node.initializer:
            yield node.initializer
        self.current_scope.define(node.name, node)
        return node

    def visit_AssignmentNode(self, node: AssignmentNode):
        yield node.expression
        current_target = node.target
        while True:
            if (
                isinstance(current_target, MemberAccessNode)
                and not current_target.pointer
            ):
                current_target = current_target.object
            else:
                break

        if isinstance(current_target, VariableNode):
            name = current_target.name
            curr = self.current_scope
            while curr is not None:
                if name in curr.symbols:
                    curr.assignments[name].append(node)
                    break
                curr = curr.parent
        else:
            yield node.target

        return node

    def visit_VariableNode(self, node: VariableNode):
        self.current_scope.record_use(node.name)
        return node

    def visit_SizeOfNode(self, node):
        if not node.is_type:
            yield node.target
        return node

    def visit_ReturnNode(self, node: ReturnNode):
        if node.expression:
            yield node.expression
        return node

    def visit_FunctionCallNode(self, node: FunctionCallNode):
        yield node.function
        for arg in node.arguments:
            yield arg
        return node

    def visit_BinaryOpNode(self, node: BinaryOpNode):
        yield node.left
        yield node.right
        return node

    def visit_UnaryOpNode(self, node: UnaryOpNode):
        yield node.operand
        return node

    def visit_ArrayAccessNode(self, node: ArrayAccessNode):
        yield node.array
        yield node.index
        return node

    def visit_CastNode(self, node: CastNode):
        yield node.expression
        return node

    def visit_MemberAccessNode(self, node: MemberAccessNode):
        yield node.object
        return node

    def visit_InitializerListNode(self, node: InitializerListNode):
        for el in node.elements:
            yield el
        return node

    def visit_EnumNode(self, node: EnumNode):
        for lbl in node.labels:
            yield lbl
        return node

    def visit_LiteralNode(self, node: LiteralNode):
        # Bezoek de mogelijke expressie achter een enum label
        for child in node.children:
            yield child
        return node


class DeadCodeEliminator(AstVisitor):
    def __init__(self):
        self.changed = False
        self.unused_decls = set()
        self.dead_assignments = set()

    def optimize(self, ast):
        analyzer = VariableUsageAnalyzer()
        analyzer.visit(ast)
        self.unused_decls = analyzer.unused_decls
        self.changed = False
        self.dead_assignments = analyzer.dead_assignments
        return self.visit(ast)

    def _contains_reachable_break(self, stmt):
        if stmt is None:
            return False
        if isinstance(stmt, BreakNode):
            return True
        if isinstance(stmt, BlockNode):
            return any(
                self._contains_reachable_break(s)
                for s in getattr(stmt, "statements", [])
            )
        if isinstance(stmt, IfNode):
            b1 = self._contains_reachable_break(getattr(stmt, "if_block", None))
            b2 = self._contains_reachable_break(getattr(stmt, "else_block", None))
            return b1 or b2
        return False

    def _must_terminate(self, stmt):
        if isinstance(stmt, (ReturnNode, BreakNode, ContinueNode)):
            return True
        if isinstance(stmt, BlockNode) and stmt.statements:
            return self._must_terminate(stmt.statements[-1])
        if (
            isinstance(stmt, IfNode)
            and getattr(stmt, "has_else", False)
            and stmt.else_block is not None
        ):
            return self._must_terminate(stmt.if_block) and self._must_terminate(
                stmt.else_block
            )

        if isinstance(stmt, WhileLoopNode):
            if isinstance(stmt.condition, LiteralNode) and stmt.condition.value != 0:
                if not self._contains_reachable_break(
                    getattr(stmt, "body_block", None)
                ):
                    return True

        return False

    def _trim(self, statements):
        out = []
        terminated = False

        for stmt in statements:
            if stmt is None:
                continue

            if terminated:
                self.changed = True
                continue

            out.append(stmt)
            if self._must_terminate(stmt):
                terminated = True

        return out

    def visit_ProgramNode(self, node: ProgramNode):
        new_headers = []
        for elem in node.header_elements:
            new_elem = yield elem
            if new_elem:
                new_headers.append(new_elem)

        return ProgramNode(
            line=node.line,
            column=node.column,
            header_elements=new_headers,
            comments=node.comments,
        )

    def visit_MainFunctionNode(self, node: MainFunctionNode):
        new_statements = []
        for stmt in node.statements:
            new_stmt = yield stmt
            if new_stmt:
                new_statements.append(new_stmt)

        return MainFunctionNode(
            node.line,
            node.column,
            self._trim(new_statements),
            comments=node.comments,
        )

    def visit_DeclarationNode(self, node: DeclarationNode):
        new_init = yield node.initializer if node.initializer else None

        if id(node) in self.unused_decls:
            self.changed = True
            if new_init and has_side_effects(new_init):
                return new_init
            return None

        return DeclarationNode(
            node.line,
            node.column,
            node.datatype,
            node.name,
            new_init,
            comments=node.comments,
        )

    def visit_AssignmentNode(self, node: AssignmentNode):
        if id(node) in self.dead_assignments:
            self.changed = True
            new_expr = yield node.expression
            if new_expr and has_side_effects(new_expr):
                return new_expr
            return None

        new_target = yield node.target
        new_expr = yield node.expression
        return AssignmentNode(
            node.line, node.column, new_target, new_expr, comments=node.comments
        )

    def visit_BlockNode(self, node: BlockNode):
        new_statements = []
        for stmt in node.statements:
            new_stmt = yield stmt
            if new_stmt:
                new_statements.append(new_stmt)
        return BlockNode(
            node.line, node.column, self._trim(new_statements), comments=node.comments
        )

    def visit_IfNode(self, node: IfNode):
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

    def visit_WhileLoopNode(self, node: WhileLoopNode):
        new_condition = yield node.condition if node.condition else None
        new_body_block = yield node.body_block if node.body_block else None
        return WhileLoopNode(
            node.line,
            node.column,
            new_condition,
            new_body_block,
            comments=node.comments,
        )

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
            new_args.append((yield arg))

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

    def visit_VariableNode(self, node: VariableNode):
        return node

    def visit_LiteralNode(self, node: LiteralNode):
        return node

    def visit_FunctionNode(self, node: FunctionNode):
        new_statements = None
        if getattr(node, "is_definition", False) and node.statements is not None:
            collected = []
            for stmt in node.statements:
                new_stmt = yield stmt
                if new_stmt:
                    collected.append(new_stmt)
            new_statements = self._trim(collected)

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

    def visit_BreakNode(self, node: BreakNode):
        return node

    def visit_ContinueNode(self, node: ContinueNode):
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

    def visit_IncludeNode(self, node: IncludeNode):
        return node

    def visit_TypedefNode(self, node: TypedefNode):
        return node
