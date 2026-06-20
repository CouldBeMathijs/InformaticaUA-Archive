import graphviz
import os
from src.parser.AST.Node import BlockNode, MemberAccessNode
from src.parser.AST.Visitor import AstVisitor


class VisualizerVisitor(AstVisitor):
    def __init__(self, show_comments=True):
        self.show_comments = show_comments
        self.dot = graphviz.Digraph(
            comment="AST",
            graph_attr={
                "rankdir": "TB",
                "splines": "spline",
                "nodesep": "0.5",
                "ranksep": "0.3",
                "overlap": "false",
                "bgcolor": "#0f1221",
                "fontname": "Helvetica",
                "forcelabels": "true",
            },
            node_attr={
                "fontname": "Helvetica",
                "style": "filled",
                "color": "#cbd5f5",
                "fontcolor": "white",
                "margin": "0.2",
            },
            edge_attr={
                "color": "#cbd5f5",
                "fontname": "Helvetica",
                "fontcolor": "#cbd5f5",
                "minlen": "2",
            },
        )
        self.counter = 0
        self.scope_counter = 0
        self.scope_stack = []
        self.scope_symbols = {}
        self.scope_labels = {}

    def _next_id(self):
        self.counter += 1
        return f"n{self.counter}"

    def _next_scope_id(self):
        self.scope_counter += 1
        return f"scope_{self.scope_counter}"

    def _format_label(self, node, base_label):
        if not self.show_comments or not getattr(node, "comments", None):
            return base_label

        comment_text = "\\n".join([f"{c}" for c in node.comments])
        return f"{base_label}\\n──────────\\n{comment_text}"

    def _push_scope(self, kind, owner_id=None):
        scope_id = self._next_scope_id()
        self.scope_stack.append(scope_id)
        self.scope_symbols[scope_id] = []
        self.scope_labels[scope_id] = kind
        self._refresh_scope_label(scope_id)

        if len(self.scope_stack) > 1:
            parent_scope = self.scope_stack[-2]
            self.dot.edge(parent_scope, scope_id, style="dashed", color="#a78bfa")

        if owner_id:
            self.dot.edge(owner_id, scope_id, style="dashed", color="#a78bfa")

        return scope_id

    def _pop_scope(self):
        if self.scope_stack:
            self.scope_stack.pop()

    def _current_scope_id(self):
        return self.scope_stack[-1] if self.scope_stack else None

    def _add_symbol_to_scope(self, symbol_name):
        scope_id = self._current_scope_id()
        if scope_id is None:
            return
        self.scope_symbols[scope_id].append(symbol_name)
        self._refresh_scope_label(scope_id)

    def _refresh_scope_label(self, scope_id):
        base = self.scope_labels.get(scope_id, "scope")
        symbols = self.scope_symbols.get(scope_id, [])
        symbols_text = "\\n".join(symbols) if symbols else "(empty)"
        label = f"{base}\\n──────────\\n{symbols_text}"
        self.dot.node(
            scope_id,
            label,
            shape="note",
            fillcolor="#334155",
            color="#a78bfa",
            fontcolor="white",
        )

    # --- Structural Nodes ---

    def visit_ProgramNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Program Root")
        self.dot.node(node_id, label, shape="doubleoctagon", fillcolor="#dc2626")
        self._push_scope("global scope", owner_id=node_id)

        for element in node.header_elements:
            element_id = yield element
            if element_id:
                self.dot.edge(node_id, element_id)

        self._pop_scope()
        return node_id

    def visit_IncludeNode(self, node):
        pass

    def visit_MainFunctionNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "int main()")
        self.dot.node(node_id, label, shape="folder", fillcolor="#ea580c")
        self._add_symbol_to_scope("FUNC main")
        self._push_scope("main scope", owner_id=node_id)

        for i, stmt in enumerate(node.statements):
            stmt_id = yield stmt
            if stmt_id:
                self.dot.edge(node_id, stmt_id, xlabel=f"stmt {i}")
        self._pop_scope()
        return node_id

    def visit_FunctionNode(self, node):
        node_id = self._next_id()
        label = self._format_label(
            node, f"Function: {node.name}()\\nReturn: {node.return_type.base_type.name}"
        )
        self.dot.node(node_id, label, shape="folder", fillcolor="#ea580c")
        self._add_symbol_to_scope(f"FUNC {node.name}")
        self._push_scope(f"function scope: {node.name}", owner_id=node_id)

        for i, param in enumerate(node.parameters):
            param_id = yield param
            if param_id:
                self.dot.edge(node_id, param_id, xlabel=f"param {i}")

        for i, stmt in enumerate(node.statements):
            stmt_id = yield stmt
            if stmt_id:
                self.dot.edge(node_id, stmt_id, xlabel=f"stmt {i}")

        self._pop_scope()
        return node_id

    def visit_ParameterNode(self, node):
        node_id = self._next_id()
        label = self._format_label(
            node, f"Param: {node.name}\\nType: {node.datatype.base_type.name}"
        )
        self.dot.node(node_id, label, shape="cds", fillcolor="#0891b2")
        self._add_symbol_to_scope(f"PARAM {node.name}")
        return node_id

    def visit_StructNode(self, node):
        node_id = self._next_id()
        struct_name = node.name if node.name else "<anonymous>"
        label = self._format_label(node, f"Struct: {struct_name}")
        self.dot.node(node_id, label, shape="record", fillcolor="#0f766e")

        if node.name:
            self._add_symbol_to_scope(f"STRUCT {node.name}")

        self._push_scope(f"struct scope: {struct_name}", owner_id=node_id)
        for i, member in enumerate(node.members):
            member_id = yield member
            if member_id:
                self.dot.edge(node_id, member_id, xlabel=f"field {i}")
        self._pop_scope()
        return node_id

    def visit_StructFieldNode(self, node):
        node_id = self._next_id()
        datatype = getattr(node, "datatype", getattr(node, "type", None))
        type_text = str(datatype) if datatype is not None else "<unknown>"
        label = self._format_label(node, f"Field: {node.name}\\nType: {type_text}")
        self.dot.node(node_id, label, shape="rect", fillcolor="#0ea5a4")
        self._add_symbol_to_scope(f"FIELD {node.name}")
        return node_id

    # --- Statements & Declarations ---

    def visit_DeclarationNode(self, node):
        node_id = self._next_id()
        base_label = f"Decl: {node.name}\\nType: {node.datatype}"
        label = self._format_label(node, base_label)

        self.dot.node(node_id, label, shape="rect", fillcolor="#0891b2")
        self._add_symbol_to_scope(f"VAR {node.name}")

        if node.initializer:
            init_id = yield node.initializer
            if init_id:
                self.dot.edge(node_id, init_id, xlabel="init")
        return node_id

    def visit_TypedefNode(self, node):
        node_id = self._next_id()
        label = self._format_label(
            node, f"Typedef: {node.name}\\nTarget: {node.target_type}"
        )
        self.dot.node(node_id, label, shape="component", fillcolor="#0ea5e9")
        self._add_symbol_to_scope(f"TYPEDEF {node.name}")
        return node_id

    def visit_AssignmentNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Assignment (=)")
        self.dot.node(node_id, label, shape="rect", fillcolor="#0284c7")

        target_id = yield node.target
        expr_id = yield node.expression

        if target_id:
            self.dot.edge(node_id, target_id, xlabel="target")
        if expr_id:
            self.dot.edge(node_id, expr_id, xlabel="value")
        return node_id

    def visit_BlockNode(self, node: BlockNode):
        node_id = self._next_id()
        if node.create_scope:
            label = self._format_label(node, "Block Scope { }")
        else:
            label = self._format_label(node, "Unscoped Block")

        self.dot.node(node_id, label, shape="folder", fillcolor="#d97706")
        self._push_scope("block scope", owner_id=node_id)

        for i, stmt in enumerate(node.statements):
            stmt_id = yield stmt
            if stmt_id:
                self.dot.edge(node_id, stmt_id, xlabel=f"stmt {i}")

        self._pop_scope()
        return node_id

    def visit_IfNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "If Statement")
        self.dot.node(node_id, label, shape="diamond", fillcolor="#eab308")

        cond_id = yield node.condition
        if cond_id:
            self.dot.edge(node_id, cond_id, xlabel="condition")

        if_block_id = yield node.if_block
        if if_block_id:
            self.dot.edge(node_id, if_block_id, xlabel="if true")

        if node.has_else:
            else_block_id = yield node.else_block
            if else_block_id:
                self.dot.edge(node_id, else_block_id, xlabel="else")

        return node_id

    # --- Expression Nodes ---

    def visit_ArrayAccessNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Array Access []")
        self.dot.node(node_id, label, shape="box", fillcolor="#4338ca")

        arr_id = yield node.array
        idx_id = yield node.index

        if arr_id:
            self.dot.edge(node_id, arr_id, xlabel="arr")
        if idx_id:
            self.dot.edge(node_id, idx_id, xlabel="idx")
        return node_id

    def visit_InitializerListNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Initializer List")
        self.dot.node(node_id, label, shape="box", fillcolor="#4338ca")

        i = 0
        for element in node.elements:
            element_id = yield element
            if element_id:
                self.dot.edge(node_id, element_id, xlabel=f"el {i}")
            i += 1

        return node_id

    def visit_FunctionCallNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Function Call ()")
        self.dot.node(node_id, label, shape="component", fillcolor="#be185d")

        func_id = yield node.function
        if func_id:
            self.dot.edge(node_id, func_id, xlabel="func")

        for i, arg in enumerate(node.arguments):
            arg_id = yield arg
            if arg_id:
                self.dot.edge(node_id, arg_id, xlabel=f"arg {i}")
        return node_id

    def visit_BinaryOpNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, f"Binary Op\\n{node.operator}")
        self.dot.node(node_id, label, shape="box", fillcolor="#7c3aed")

        left_id = yield node.left
        right_id = yield node.right

        if left_id:
            self.dot.edge(node_id, left_id, xlabel="L")
        if right_id:
            self.dot.edge(node_id, right_id, xlabel="R")
        return node_id

    def visit_UnaryOpNode(self, node):
        node_id = self._next_id()
        kind = "Postfix" if node.postfix else "Prefix"
        label = self._format_label(node, f"{kind}\\n{node.operator}")
        self.dot.node(node_id, label, shape="box", fillcolor="#9333ea")

        operand_id = yield node.operand
        if operand_id:
            self.dot.edge(node_id, operand_id)
        return node_id

    def visit_CastNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, f"Cast to {node.target_type}")
        self.dot.node(node_id, label, shape="parallelogram", fillcolor="#4f46e5")

        expr_id = yield node.expression
        if expr_id:
            self.dot.edge(node_id, expr_id)
        return node_id

    def visit_EnumNode(self, node):
        node_id = self._next_id()
        base_label = f"enum {node.name}" if node.name else "enum"
        label = self._format_label(node, base_label)
        self.dot.node(node_id, label, shape="tab", fillcolor="#7e22ce")
        if node.name:
            self._add_symbol_to_scope(f"ENUM {node.name}")

        # Render all the labels as children
        for i, label_node in enumerate(node.labels):
            lbl_id = yield label_node
            if lbl_id:
                self.dot.edge(node_id, lbl_id, xlabel=f"label {i}")
            if hasattr(label_node, "enum_label") and label_node.enum_label:
                self._add_symbol_to_scope(f"ENUM_LABEL {label_node.enum_label}")
        return node_id

    def visit_WhileLoopNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, f"while ({node.condition})")
        self.dot.node(node_id, label, shape="diamond", fillcolor="#eab308")
        if node.condition:
            cond_id = yield node.condition
            self.dot.edge(node_id, cond_id, xlabel="condition")

        if node.body_block:
            body_node_id = yield node.body_block
            self.dot.edge(node_id, body_node_id, xlabel="body")
        return node_id

    # --- Leaf Nodes ---

    def visit_LiteralNode(self, node):
        node_id = self._next_id()

        base_label = f"Literal ({node.datatype})\\n{node}"
        label = self._format_label(node, base_label)

        color = "#16a34a"
        self.dot.node(node_id, label, shape="ellipse", fillcolor=color)
        return node_id

    def visit_VariableNode(self, node):
        node_id = self._next_id()
        label = self._format_label(
            node, f"Var: {node.name}\\n Inferred Type: {node.inferred_type}"
        )
        self.dot.node(node_id, label, shape="hexagon", fillcolor="#2563eb")
        return node_id

    def visit_ReturnNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Return")
        self.dot.node(node_id, label, shape="ellipse", fillcolor="#ef4444")

        if node.expression:
            expr_id = yield node.expression
            if expr_id:
                self.dot.edge(node_id, expr_id, xlabel="value")
        return node_id

    def visit_BreakNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Break")
        self.dot.node(node_id, label, shape="octagon", fillcolor="#ec4899")
        return node_id

    def visit_ContinueNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "Continue")
        self.dot.node(node_id, label, shape="octagon", fillcolor="#d946ef")
        return node_id

    def visit_MemberAccessNode(self, node: MemberAccessNode):
        node_id = self._next_id()
        operator = "->" if node.pointer else "."

        base_label = f"Member Access ({operator})\\nField: {node.field}"
        label = self._format_label(node, base_label)

        self.dot.node(node_id, label, shape="box", fillcolor="#0284c7")
        if node.object:
            obj_id = yield node.object
            if obj_id:
                self.dot.edge(node_id, obj_id, xlabel="object")

        return node_id

    def visit_SizeOfNode(self, node):
        node_id = self._next_id()
        label = self._format_label(node, "sizeof")
        self.dot.node(node_id, label, shape="box", fillcolor="#4f46e5")
        if not node.is_type:
            target_id = yield node.target
            if target_id:
                self.dot.edge(node_id, target_id)
        else:
            type_id = self._next_id()
            self.dot.node(
                type_id, str(node.target), shape="ellipse", fillcolor="#16a34a"
            )
            self.dot.edge(node_id, type_id)
        return node_id

    # --- Fallback ---

    def visit_default(self, node):
        raise Exception("Cannot visualize: unknown node type")

    # --- IO ---

    def render(self, filepath):
        base_path, extension = os.path.splitext(filepath)

        fmt = extension.strip(".").lower() if extension else "pdf"

        try:
            # Graphviz will save to base_path + "." + fmt
            output_path = self.dot.render(base_path, format=fmt, cleanup=True)
            print(f"[ INFO ] AST visualized at {output_path}")
        except Exception as e:
            print(f"[ ERROR ] Could not render file: {e}")
