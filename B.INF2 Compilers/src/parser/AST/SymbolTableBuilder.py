from src.parser.AST.Visitor import AstVisitor
from src.parser.AST.Node import *
import src.issue_printer as ispr
from copy import deepcopy
from html import escape
import graphviz


class Symbol:
    """Represents a named entity in the code (Variable, Function, Enum, etc.)"""

    def __init__(
        self,
        name,
        kind,
        type_info,
        params=None,
        value=None,
        is_definition=False,
        is_vararg=False,
        is_implicit=False,
        original_name=None,
    ):
        self.name = name
        self.original_name = original_name or name
        self.kind = kind  # 'VAR', 'FUNC', 'PARAM', 'ENUM_LABEL'
        self.type_info = type_info  # Expects a TypeNode or BaseType
        self.params = params or []  # List of TypeNodes (for functions)
        self.value = value  # Compile-time value (used for enum labels)
        self.is_definition = is_definition
        self.is_vararg = is_vararg
        self.is_implicit = is_implicit

    def __str__(self):
        return f"<{self.kind} {self.name} : {self.type_info}>"


class Scope:
    """Represents a lexical scope (e.g., global, function body, block)"""

    def __init__(self, parent_scope=None, scope_id=0):
        self.scope_id = scope_id
        self.symbols = {}
        self.parent = parent_scope
        self.children = []
        if parent_scope is not None:
            parent_scope.children.append(self)

    def define(self, symbol: Symbol):
        if symbol.name in self.symbols:
            return False  # Symbol already defined in THIS scope
        self.symbols[symbol.name] = symbol
        return True

    def lookup(self, name) -> Symbol | None:
        # Look in current scope
        if name in self.symbols:
            return self.symbols[name]
        # Recursively look in parent scope
        if self.parent:
            return self.parent.lookup(name)
        return None

    def lookup_by_original_name(self, original_name) -> list[Symbol]:
        """Returns all symbols with the same original name in this scope and parents."""
        matches = []
        for sym in self.symbols.values():
            if sym.original_name == original_name:
                matches.append(sym)
        if self.parent:
            matches.extend(self.parent.lookup_by_original_name(original_name))
        return matches


class SymbolTable:
    """Manages the current scope and the global scope."""

    def __init__(self):
        self._next_scope_id = 1
        self.global_scope = Scope(scope_id=0)
        self.current_scope = self.global_scope
        self.all_scopes = [self.global_scope]

    def _add_builtins(self, library):
        int_type = TypeNode(0, 0, BaseType.INT)
        char_ptr_type = TypeNode(0, 0, BaseType.CHAR, ptr_depth=1)
        void_type = TypeNode(0, 0, BaseType.VOID)
        void_ptr_type = TypeNode(0, 0, BaseType.VOID, ptr_depth=1)
        file_ptr_type = TypeNode(
            0, 0, BaseType.STRUCT, ptr_depth=1, struct_name="_IO_FILE"
        )

        if library == "stdio.h":
            if self.global_scope.lookup("FILE") is None:
                actual_struct_sym = Symbol(
                    name="_IO_FILE",
                    kind="STRUCT",
                    type_info=TypeNode(0, 0, BaseType.STRUCT, struct_name="_IO_FILE"),
                    value={},
                    is_definition=True,
                )
                self.global_scope.define(actual_struct_sym)

                file_typedef_sym = Symbol(
                    name="FILE",
                    kind="TYPEDEF",
                    type_info=TypeNode(0, 0, BaseType.STRUCT, struct_name="_IO_FILE"),
                    is_definition=True,
                )
                self.global_scope.define(file_typedef_sym)

            builtins_stdio = [
                Symbol(
                    "printf",
                    "FUNC",
                    int_type,
                    [char_ptr_type],
                    is_definition=True,
                    is_vararg=True,
                ),
                Symbol(
                    "scanf",
                    "FUNC",
                    int_type,
                    [char_ptr_type],
                    is_definition=True,
                    is_vararg=True,
                ),
                Symbol(
                    "fopen",
                    "FUNC",
                    file_ptr_type,
                    [char_ptr_type, char_ptr_type],
                    is_definition=True,
                ),
                Symbol("fclose", "FUNC", int_type, [file_ptr_type], is_definition=True),
                Symbol(
                    "fgets",
                    "FUNC",
                    char_ptr_type,
                    [char_ptr_type, int_type, file_ptr_type],
                    is_definition=True,
                ),
                Symbol(
                    "fputs",
                    "FUNC",
                    int_type,
                    [char_ptr_type, file_ptr_type],
                    is_definition=True,
                ),
            ]
            for sym in builtins_stdio:
                if self.global_scope.lookup(sym.name) is None:
                    self.global_scope.define(sym)

        elif library == "stdlib.h":
            builtins_stdlib = [
                Symbol("malloc", "FUNC", void_ptr_type, [int_type], is_definition=True),
                Symbol(
                    "calloc",
                    "FUNC",
                    void_ptr_type,
                    [int_type, int_type],
                    is_definition=True,
                ),
                Symbol("free", "FUNC", void_type, [void_ptr_type], is_definition=True),
                Symbol(
                    "realloc",
                    "FUNC",
                    void_ptr_type,
                    [void_ptr_type, int_type],
                    is_definition=True,
                ),
            ]
            for sym in builtins_stdlib:
                if self.global_scope.lookup(sym.name) is None:
                    self.global_scope.define(sym)

    def __str__(self):
        output = ["\n" + "=" * 40, "SYMBOL TABLE", "=" * 40]

        scope_to_print = self.current_scope
        depth = 0

        while scope_to_print is not None:
            scope_label = (
                "GLOBAL SCOPE"
                if scope_to_print == self.global_scope
                else f"LOCAL SCOPE (Depth {depth})"
            )
            output.append(f"\n[{scope_label}]")

            if not scope_to_print.symbols:
                output.append("  (no symbols defined)")
            else:
                for name in sorted(scope_to_print.symbols.keys()):
                    sym = scope_to_print.symbols[name]

                    extra = ""
                    if sym.kind == "FUNC":
                        param_list = ", ".join([str(p) for p in sym.params])
                        extra = f" | Params: [{param_list}]"
                        if sym.is_vararg:
                            extra += " ..."
                    elif sym.kind == "ENUM_LABEL":
                        extra = f" | Value: {sym.value}"

                    output.append(
                        f"  {name:<12} : {sym.kind:<10} | Type: {sym.type_info}{extra}"
                    )

            scope_to_print = scope_to_print.parent
            depth += 1

        output.append("=" * 40 + "\n")
        return "\n".join(output)

    def enter_scope(self):
        new_scope = Scope(parent_scope=self.current_scope, scope_id=self._next_scope_id)
        self._next_scope_id += 1
        self.all_scopes.append(new_scope)
        self.current_scope = new_scope

    def exit_scope(self):
        if self.current_scope.parent is not None:
            self.current_scope = self.current_scope.parent
        else:
            raise Exception("Cannot exit global scope.")

    def define(self, symbol):
        return self.current_scope.define(symbol)

    def lookup(self, name) -> Symbol | None:
        return self.current_scope.lookup(name)

    def lookup_by_original_name(self, original_name) -> list[Symbol]:
        return self.current_scope.lookup_by_original_name(original_name)

    def lookup_tag(self, name) -> Symbol | None:
        scope = self.current_scope
        while scope is not None:
            sym = scope.symbols.get(name)
            if sym is not None and sym.kind == "STRUCT":
                return sym
            scope = scope.parent
        return None

    def to_dot(self) -> str:
        dot = graphviz.Digraph(
            name="SymbolTable",
            graph_attr={"rankdir": "TB", "bgcolor": "#0f1221", "fontname": "Helvetica"},
            node_attr={"shape": "plain", "fontname": "Helvetica", "fontcolor": "white"},
            edge_attr={"color": "#94a3b8", "fontcolor": "#cbd5f5"},
        )

        for scope in self.all_scopes:
            title = (
                "GLOBAL SCOPE"
                if scope == self.global_scope
                else f"LOCAL SCOPE #{scope.scope_id}"
            )
            title = escape(title)

            rows = [
                '<TR><TD COLSPAN="3" BGCOLOR="#1f2937"><FONT COLOR="white"><B>'
                + title
                + "</B></FONT></TD></TR>",
                '<TR><TD BGCOLOR="#334155"><FONT COLOR="white"><B>Name</B></FONT></TD>'
                '<TD BGCOLOR="#334155"><FONT COLOR="white"><B>Kind</B></FONT></TD>'
                '<TD BGCOLOR="#334155"><FONT COLOR="white"><B>Type / Extra</B></FONT></TD></TR>',
            ]

            if scope.symbols:
                for name in sorted(scope.symbols.keys()):
                    sym = scope.symbols[name]
                    extra = ""
                    if sym.kind == "FUNC":
                        param_list = ", ".join([str(p) for p in sym.params])
                        extra = f"params: [{param_list}]"
                        if sym.is_vararg:
                            extra += ", vararg"
                    elif sym.kind == "ENUM_LABEL":
                        extra = f"value: {sym.value}"

                    type_and_extra = str(sym.type_info)
                    if extra:
                        type_and_extra += f" ({extra})"

                    rows.append(
                        "<TR>"
                        f'<TD ALIGN="LEFT" BGCOLOR="#1e293b"><FONT COLOR="white">{escape(name)}</FONT></TD>'
                        f'<TD ALIGN="LEFT" BGCOLOR="#1e293b"><FONT COLOR="white">{escape(sym.kind)}</FONT></TD>'
                        f'<TD ALIGN="LEFT" BGCOLOR="#1e293b"><FONT COLOR="white">{escape(type_and_extra)}</FONT></TD>'
                        "</TR>"
                    )
            else:
                rows.append(
                    '<TR><TD COLSPAN="3" ALIGN="LEFT"><FONT COLOR="#94a3b8">(no symbols)</FONT></TD></TR>'
                )

            html_label = (
                '<<TABLE BORDER="0" CELLBORDER="1" CELLSPACING="0" CELLPADDING="6">'
                + "".join(rows)
                + "</TABLE>>"
            )
            dot.node(f"scope_{scope.scope_id}", label=html_label)

        for scope in self.all_scopes:
            if scope.parent is not None:
                dot.edge(
                    f"scope_{scope.parent.scope_id}",
                    f"scope_{scope.scope_id}",
                )

        return dot.source


class SymbolTableBuilder(AstVisitor):
    """AST Visitor that builds the symbol table and collects scope/naming issues."""

    def __init__(self, issue_collector=None):
        self.symtab = SymbolTable()
        self.issues = issue_collector if issue_collector is not None else []
        self.current_function = None
        self.called_functions = []
        self.loop_depth = 0

    def _resolve_type_node(
        self, typ: TypeNode, issue_node=None, resolving: set[str] | None = None
    ):
        if typ is None or not isinstance(typ, TypeNode):
            return typ, True

        alias_name = getattr(typ, "alias_name", None)
        if not alias_name:
            base = getattr(typ, "base_type", None)
            if base in (BaseType.STRUCT, BaseType.UNION) and getattr(
                typ, "struct_name", None
            ):
                tag_sym = self.symtab.lookup_tag(typ.struct_name)
                if tag_sym is not None and tag_sym.kind == "STRUCT":
                    if tag_sym.type_info.base_type != base:
                        if issue_node is not None:
                            self.issues.append(
                                ispr.type_mismatch_error(
                                    issue_node,
                                    tag_sym.type_info.base_type.name.lower(),
                                    base.name.lower(),
                                )
                            )
            return typ, True

        if resolving is None:
            resolving = set()

        if alias_name in resolving:
            if issue_node is not None:
                self.issues.append(
                    ispr.type_mismatch_error(
                        issue_node, "non-cyclic typedef", alias_name
                    )
                )
            return typ, False

        alias_symbol = self.symtab.lookup(alias_name)
        if alias_symbol is None or alias_symbol.kind != "TYPEDEF":
            if issue_node is not None:
                self.issues.append(
                    ispr.type_mismatch_error(issue_node, "defined type", alias_name)
                )
            return typ, False

        base_target, ok = self._resolve_type_node(
            alias_symbol.type_info, issue_node, resolving | {alias_name}
        )

        if not ok or not isinstance(base_target, TypeNode):
            return typ, False

        resolved = deepcopy(base_target)
        resolved.line = typ.line
        resolved.column = typ.column
        resolved.is_const = getattr(base_target, "is_const", False) or getattr(
            typ, "is_const", False
        )
        base_ptr_quals = list(getattr(base_target, "ptr_const_quals", []))
        typ_ptr_quals = list(getattr(typ, "ptr_const_quals", []))
        resolved.ptr_depth = getattr(base_target, "ptr_depth", 0) + getattr(
            typ, "ptr_depth", 0
        )
        resolved.ptr_const_quals = base_ptr_quals + typ_ptr_quals
        resolved.array_dimensions = getattr(typ, "array_dimensions", None)
        resolved.alias_name = None

        resolved_base = getattr(resolved, "base_type", None)
        if resolved_base in (BaseType.STRUCT, BaseType.UNION) and getattr(
            resolved, "struct_name", None
        ):
            tag_sym = self.symtab.lookup_tag(resolved.struct_name)
            if tag_sym is not None and tag_sym.kind == "STRUCT":
                if tag_sym.type_info.base_type != resolved_base:
                    if issue_node is not None:
                        self.issues.append(
                            ispr.type_mismatch_error(
                                issue_node,
                                tag_sym.type_info.base_type.name.lower(),
                                resolved_base.name.lower(),
                            )
                        )

        return resolved, True

    def _guarantees_return(self, node) -> bool:
        if node is None:
            return False

        if isinstance(node, ReturnNode):
            return True

        if isinstance(node, BlockNode):
            for stmt in node.statements:
                if self._guarantees_return(stmt):
                    return True
            return False

        if isinstance(node, IfNode):
            if node.has_else:
                if_returns = self._guarantees_return(node.if_block)
                else_returns = self._guarantees_return(node.else_block)
                return if_returns and else_returns
            return False

        if isinstance(node, WhileLoopNode):
            if isinstance(node.condition, LiteralNode):
                try:
                    if int(node.condition.value) != 0:
                        return True
                except (ValueError, TypeError):
                    pass
            return False

        return False

    def _eval_enum_expr(self, expr) -> int:
        if isinstance(expr, LiteralNode):
            if isinstance(expr.value, Node):
                return self._eval_enum_expr(expr.value)
            return int(expr.value)

        if isinstance(expr, VariableNode):
            sym = self.symtab.lookup(expr.name)
            if sym is None:
                raise ValueError(expr.name)
            if sym.kind == "ENUM_LABEL" and sym.value is not None:
                return int(sym.value)
            raise ValueError(expr.name)

        if isinstance(expr, UnaryOpNode):
            val = self._eval_enum_expr(expr.operand)
            if expr.operator == "+":
                return +val
            if expr.operator == "-":
                return -val
            if expr.operator == "~":
                return ~val
            if expr.operator == "!":
                return int(not val)
            raise ValueError("Unsupported unary operator")

        if isinstance(expr, CastNode):
            return int(self._eval_enum_expr(expr.expression))

        if isinstance(expr, BinaryOpNode):
            left = self._eval_enum_expr(expr.left)
            right = self._eval_enum_expr(expr.right)
            op = expr.operator
            if op == "+":
                return left + right
            if op == "-":
                return left - right
            if op == "*":
                return left * right
            if op == "/":
                return left // right
            if op == "%":
                return left % right
            if op == "<<":
                return left << right
            if op == ">>":
                return left >> right
            if op == "&":
                return left & right
            if op == "|":
                return left | right
            if op == "^":
                return left ^ right
            if op == "&&":
                return int(bool(left) and bool(right))
            if op == "||":
                return int(bool(left) or bool(right))
            if op == "==":
                return int(left == right)
            if op == "!=":
                return int(left != right)
            if op == "<":
                return int(left < right)
            if op == "<=":
                return int(left <= right)
            if op == ">":
                return int(left > right)
            if op == ">=":
                return int(left >= right)
            raise ValueError("Unsupported binary operator")

        raise ValueError("Unsupported enum expression")

    def _mangle_function(self, name, param_types):
        if name == "main":
            return name
        # Builtins should not be mangled
        builtins = {
            "printf",
            "scanf",
            "malloc",
            "calloc",
            "free",
            "realloc",
            "fopen",
            "fclose",
            "fgets",
            "fputs",
        }
        if name in builtins:
            return name

        mangled = name
        for t in param_types:
            base = t.base_type.name.lower()
            ptr = t.ptr_depth
            mangled += f"_{base}_p{ptr}"
        return mangled

    def generic_visit(self, node):
        """Fallback for nodes that just need their children traversed."""
        for child in node.children:
            if child is not None:
                yield child

    def _eval_array_dim_expr(self, expr) -> int:
        if isinstance(expr, VariableNode):
            sym = self.symtab.lookup(expr.name)
            if sym is not None and sym.kind == "VAR" and sym.value is not None:
                return int(sym.value)
        return self._eval_enum_expr(expr)

    # --- Structural Visitors ---

    def visit_IncludeNode(self, node: IncludeNode):
        if not node.system:  # Normal includes should have been handled by preprocessor
            return

        valid_headers = {"stdio.h", "stdlib.h"}

        if node.file not in valid_headers:
            self.issues.append(ispr.non_existant_file_error(node, node.file))
            return

        self.symtab._add_builtins(node.file)

    def visit_ProgramNode(self, node: ProgramNode):
        for elem in node.header_elements:
            yield elem

        for call_node, func_name in self.called_functions:
            sym = self.symtab.global_scope.lookup(func_name)
            if sym is not None and sym.kind == "FUNC" and not sym.is_definition:
                if getattr(sym, "is_implicit", False) and func_name in {
                    "printf",
                    "scanf",
                }:
                    continue

                self.issues.append(ispr.undeclared_function_error(call_node, func_name))

    def visit_MainFunctionNode(self, node: MainFunctionNode):
        existing = self.symtab.global_scope.symbols.get("main")
        if existing is not None and existing.kind == "TYPEDEF":
            self.issues.append(ispr.already_declared_error(node, "main"))

        previous_function = self.current_function
        self.current_function = type(
            "_MainContext",
            (),
            {
                "name": "main",
                "return_type": TypeNode(node.line, node.column, BaseType.INT),
            },
        )()
        self.symtab.enter_scope()
        yield from self.generic_visit(node)
        self.symtab.exit_scope()
        self.current_function = previous_function

    def visit_FunctionNode(self, node: FunctionNode):
        resolved_return, _ = self._resolve_type_node(node.return_type, node)
        if isinstance(resolved_return, TypeNode):
            node.return_type = resolved_return

        for param in node.parameters:
            resolved_param_type, _ = self._resolve_type_node(param.datatype, param)
            if isinstance(resolved_param_type, TypeNode):
                param.datatype = resolved_param_type

        mangled_name = self._mangle_function(node.name, node.parameter_types)
        node.mangled_name = mangled_name

        func_symbol = Symbol(
            name=mangled_name,
            original_name=node.name,
            kind="FUNC",
            type_info=node.return_type,
            params=node.parameter_types,
            is_definition=node.is_definition,
        )

        # Check for conflicts with same original name
        existing_with_same_name = self.symtab.lookup_by_original_name(node.name)
        for existing in existing_with_same_name:
            if existing.kind != "FUNC":
                self.issues.append(ispr.already_declared_error(node, node.name))
                break

            if existing.name == mangled_name:
                if existing.type_info != node.return_type:
                    self.issues.append(
                        ispr.already_declared_error(
                            node, f"conflicting types for '{node.name}'"
                        )
                    )
                elif node.is_definition and existing.is_definition:
                    self.issues.append(
                        ispr.already_declared_error(node, f"function '{node.name}'")
                    )
                elif node.is_definition:
                    existing.is_definition = True

                break
            else:
                if not node.is_definition or not existing.is_definition:
                    self.issues.append(
                        ispr.already_declared_error(
                            node, f"conflicting types for '{node.name}'"
                        )
                    )

        if not any(
            e.name == mangled_name for e in existing_with_same_name if e.kind == "FUNC"
        ):
            self.symtab.define(func_symbol)

        previous_function = self.current_function
        self.current_function = node
        self.symtab.enter_scope()

        # yield from node.parameters
        for param in node.parameters:
            yield param

        if node.is_definition:
            for stmt in node.statements:
                yield stmt

            fn_ret = node.return_type
            fn_is_void = (
                getattr(fn_ret, "base_type", None) == BaseType.VOID
                and getattr(fn_ret, "ptr_depth", 0) == 0
            )

            if not fn_is_void:
                has_guaranteed_return = False
                for stmt in node.statements:
                    if self._guarantees_return(stmt):
                        has_guaranteed_return = True
                        break

                if not has_guaranteed_return:
                    self.issues.append(ispr.missing_return_error(node))
        self.symtab.exit_scope()
        self.current_function = previous_function

    def visit_ParameterNode(self, node: ParameterNode):
        resolved_type, _ = self._resolve_type_node(node.datatype, node)
        if isinstance(resolved_type, TypeNode):
            node.datatype = resolved_type

        param_symbol = Symbol(node.name, "PARAM", node.datatype)
        if not self.symtab.define(param_symbol):
            self.issues.append(
                ispr.already_declared_error(node, f"parameter {node.name}")
            )

    def visit_EnumNode(self, node: EnumNode):
        # In C, enum labels pollute the enclosing scope. We add them directly to the current scope.
        next_enum_value = 0
        for lbl in node.labels:
            if isinstance(lbl, LiteralNode):
                # The label's name is stored in enum_label
                if lbl.value is None:
                    enum_value = next_enum_value
                else:
                    try:
                        enum_value = self._eval_enum_expr(lbl.value)
                    except Exception:
                        enum_value = next_enum_value

                sym = Symbol(
                    lbl.enum_label,
                    "ENUM_LABEL",
                    lbl.datatype,
                    value=enum_value,
                )
                if not self.symtab.define(sym):
                    self.issues.append(
                        ispr.already_declared_error(node, f"enum label {node.name}")
                    )
                next_enum_value = enum_value + 1

        yield from self.generic_visit(node)

    # --- Statement & Scoping Visitors ---

    def visit_BlockNode(self, node: BlockNode):
        self.symtab.enter_scope()
        yield from self.generic_visit(node)
        self.symtab.exit_scope()

    def visit_IfNode(self, node: IfNode):
        yield from self.generic_visit(node)

    def _is_assignable_lvalue(self, target):
        if isinstance(target, ArrayAccessNode):
            return True
        if isinstance(target, UnaryOpNode) and target.operator == "*":
            return True
        if isinstance(target, MemberAccessNode):
            return True
        if isinstance(target, VariableNode):
            sym = self.symtab.lookup(target.name)
            return sym is not None and sym.kind != "ENUM_LABEL"
        return False

    def _is_const_qualified_lvalue_type(self, typ: TypeNode | None) -> bool:
        if typ is None:
            return False
        if getattr(typ, "ptr_depth", 0) == 0:
            return bool(getattr(typ, "is_const", False))
        ptr_quals = list(getattr(typ, "ptr_const_quals", []))
        if not ptr_quals:
            return False
        return bool(ptr_quals[-1])

    def visit_DeclarationNode(self, node: DeclarationNode):
        const_value = None
        ambiguous_mul_stmt = (
            getattr(node.datatype, "alias_name", None) is not None
            and getattr(node.datatype, "ptr_depth", 0) == 1
            and node.initializer is None
            and getattr(node.datatype, "array_dimensions", None) is None
        )
        if ambiguous_mul_stmt:
            alias_name = node.datatype.alias_name
            alias_sym = self.symtab.lookup(alias_name)
            if alias_sym is None or alias_sym.kind != "TYPEDEF":
                lhs = self.symtab.lookup(alias_name)
                rhs = self.symtab.lookup(node.name)
                if lhs is None:
                    self.issues.append(ispr.unknown_variable_error(node, alias_name))
                if rhs is None:
                    self.issues.append(ispr.unknown_variable_error(node, node.name))
                if lhs is not None and rhs is not None:
                    lhs_type = lhs.type_info
                    rhs_type = rhs.type_info
                    lhs_is_ptr = getattr(lhs_type, "ptr_depth", 0) > 0 or (
                        getattr(lhs_type, "array_dimensions", None) is not None
                    )
                    rhs_is_ptr = getattr(rhs_type, "ptr_depth", 0) > 0 or (
                        getattr(rhs_type, "array_dimensions", None) is not None
                    )
                    if lhs_is_ptr or rhs_is_ptr:
                        self.issues.append(
                            ispr.invalid_binary_operands_error(node, lhs_type, rhs_type)
                        )
                return

        resolved_decl_type, _ = self._resolve_type_node(node.datatype, node)
        if isinstance(resolved_decl_type, TypeNode):
            node.datatype = resolved_decl_type

        var_symbol = Symbol(node.name, "VAR", node.datatype, value=const_value)
        if not self.symtab.define(var_symbol):
            self.issues.append(ispr.already_declared_error(node, node.name))

        if getattr(node.datatype, "array_dimensions", None):
            for dim in node.datatype.array_dimensions:
                if dim is not None:
                    _ = yield dim
                    dim_type = getattr(dim, "inferred_type", None)
                    if dim_type is not None:
                        if (
                            dim_type.base_type != BaseType.INT
                            or getattr(dim_type, "ptr_depth", 0) > 0
                        ):
                            self.issues.append(
                                ispr.type_mismatch_error(node, "integer", dim_type)
                            )
                    try:
                        dim_val = self._eval_array_dim_expr(dim)
                        if dim_val < 0:
                            self.issues.append(
                                ispr.type_mismatch_error(
                                    dim,
                                    "array size greater than or equal to zero",
                                    dim_val,
                                )
                            )
                    except Exception:
                        pass

        if self.symtab.current_scope == self.symtab.global_scope and node.initializer:
            if not isinstance(node.initializer, (LiteralNode, InitializerListNode)):
                try:
                    self._eval_enum_expr(node.initializer)
                except Exception:
                    self.issues.append(ispr.global_initializer_not_constant_error(node))

        # Visit initializer first so right-hand side is evaluated before variable is defined
        # (prevents `int x = x;` from passing cleanly if x wasn't already in an outer scope)
        if node.initializer:
            _ = yield node.initializer

            init_type = getattr(node.initializer, "inferred_type", None)
            if node.datatype is not None and init_type is not None:
                left_depth = getattr(node.datatype, "ptr_depth", 0)
                right_depth = getattr(init_type, "ptr_depth", 0)
                left_is_array = (
                    getattr(node.datatype, "array_dimensions", None) is not None
                )

                if left_depth == 0 and right_depth > 0:
                    if not left_is_array:
                        if getattr(node.datatype, "base_type", None) == BaseType.FLOAT:
                            self.issues.append(
                                ispr.type_mismatch_error(node, node.datatype, init_type)
                            )
                        else:
                            self.issues.append(
                                ispr.incompatible_pointer_types_warning(
                                    node, node.datatype, init_type
                                )
                            )
                if left_depth > 0 and right_depth > 0:
                    right_pointee_is_const = getattr(init_type, "is_const", False)
                    left_pointee_is_const = getattr(node.datatype, "is_const", False)

                    if right_pointee_is_const and not left_pointee_is_const:
                        self.issues.append(
                            ispr.discarded_const_qualifier_warning(
                                node, node.datatype, init_type
                            )
                        )

        const_value = None
        if node.initializer is not None:
            try:
                const_value = self._eval_enum_expr(node.initializer)
            except Exception:
                const_value = None
            # If we could determine a compile-time constant value for this variable,
            # store it on the symbol so array-dimension evaluation can use it.
            if const_value is not None:
                sym = self.symtab.lookup(node.name)
                if sym is not None and sym.kind == "VAR":
                    sym.value = const_value

    def visit_AssignmentNode(self, node: AssignmentNode):
        _ = yield node.target
        _ = yield node.expression

        target_type = getattr(node.target, "inferred_type", None)
        expr_type = getattr(node.expression, "inferred_type", None)

        if target_type is not None and expr_type is not None:
            left_depth = getattr(target_type, "ptr_depth", 0)
            right_depth = getattr(expr_type, "ptr_depth", 0)

            if left_depth == 0 and right_depth > 0:
                self.issues.append(
                    ispr.incompatible_pointer_types_warning(
                        node, target_type, expr_type
                    )
                )
            if left_depth > 0 and right_depth > 0:
                right_pointee_is_const = getattr(expr_type, "is_const", False)
                left_pointee_is_const = getattr(target_type, "is_const", False)

                if right_pointee_is_const and not left_pointee_is_const:
                    self.issues.append(
                        ispr.discarded_const_qualifier_warning(
                            node, target_type, expr_type
                        )
                    )

        if not self._is_assignable_lvalue(node.target):
            self.issues.append(ispr.assignment_to_rvalue_error(node))
            return

        if self._is_const_qualified_lvalue_type(target_type):
            target_name = (
                node.target.name
                if isinstance(node.target, VariableNode)
                else "read-only location"
            )
            self.issues.append(ispr.const_is_invariable_error(node, target_name))

    # --- Expression & Leaf Visitors ---

    def visit_VariableNode(self, node: VariableNode):
        sym = self.symtab.lookup(node.name)
        if sym is None:
            self.issues.append(ispr.unknown_variable_error(node, node.name))
        elif sym.kind in ("TYPEDEF", "STRUCT", "UNION"):
            self.issues.append(ispr.type_mismatch_error(node, "value", "type name"))
        else:
            base_type_info = sym.type_info

            if getattr(base_type_info, "array_dimensions", None):
                decayed_type = deepcopy(base_type_info)

                dims = decayed_type.array_dimensions
                if len(dims) > 1:
                    decayed_type.array_dimensions = dims[1:]
                else:
                    decayed_type.array_dimensions = None

                decayed_type.ptr_depth += 1

                node.inferred_type = decayed_type
            else:
                node.inferred_type = base_type_info

            if sym.kind == "ENUM_LABEL":
                node.enum_value = sym.value

    def visit_UnaryOpNode(self, node: UnaryOpNode):
        _ = yield node.operand

        op_type = getattr(node.operand, "inferred_type", None)
        if op_type is not None:
            if node.operator == "&":
                if not self._is_assignable_lvalue(node.operand):
                    self.issues.append(
                        ispr.reference_of_rvalue_error(node, op_type.base_type)
                    )
                node.inferred_type = TypeNode(
                    node.line,
                    node.column,
                    op_type.base_type,
                    is_const=op_type.is_const,
                    ptr_depth=op_type.ptr_depth + 1,
                    ptr_const_quals=list(getattr(op_type, "ptr_const_quals", []))
                    + [False],
                    struct_name=getattr(op_type, "struct_name", None),
                    alias_name=getattr(op_type, "alias_name", None),
                )
            elif node.operator == "*":
                is_pointer = op_type.ptr_depth > 0
                is_array = getattr(op_type, "array_dimensions", None) is not None
                if not is_pointer and not is_array:
                    self.issues.append(ispr.dereferencing_non_pointer_error(node))

                struct_name = getattr(op_type, "struct_name", None)
                node.inferred_type = TypeNode(
                    node.line,
                    node.column,
                    op_type.base_type,
                    is_const=op_type.is_const,
                    ptr_depth=max(0, op_type.ptr_depth - 1),
                    ptr_const_quals=list(getattr(op_type, "ptr_const_quals", []))[:-1],
                    struct_name=struct_name,
                )
            elif node.operator == "~":
                is_pointer = op_type.ptr_depth > 0
                if is_pointer:
                    self.issues.append(ispr.unary_not_on_pointer_error(node))
                    node.inferred_type = TypeNode(
                        node.line,
                        node.column,
                        op_type.base_type,
                        is_const=op_type.is_const,
                        ptr_depth=op_type.ptr_depth,
                    )
            elif node.operator == "!":
                node.inferred_type = TypeNode(node.line, node.column, BaseType.INT)
            else:
                node.inferred_type = op_type

        if node.operator in ("++", "--") and not self._is_assignable_lvalue(
            node.operand
        ):
            self.issues.append(ispr.assignment_to_rvalue_error(node))
            return

        if node.operator in ("++", "--"):
            operand_type = getattr(node.operand, "inferred_type", None)
            if self._is_const_qualified_lvalue_type(operand_type):
                operand_name = (
                    node.operand.name
                    if isinstance(node.operand, VariableNode)
                    else "read-only location"
                )
                self.issues.append(ispr.const_is_invariable_error(node, operand_name))

    def visit_BinaryOpNode(self, node: BinaryOpNode):
        _ = yield node.left
        _ = yield node.right

        left_type = getattr(node.left, "inferred_type", None)
        right_type = getattr(node.right, "inferred_type", None)

        if left_type is None or right_type is None:
            return

        left_is_ptr = (
            getattr(left_type, "ptr_depth", 0) > 0
            or getattr(left_type, "array_dimensions", None) is not None
        )
        right_is_ptr = (
            getattr(right_type, "ptr_depth", 0) > 0
            or getattr(right_type, "array_dimensions", None) is not None
        )

        if left_type.base_type == BaseType.VOID and not left_is_ptr:
            self.issues.append(
                ispr.invalid_binary_operands_error(node, left_type, right_type)
            )
        elif right_type.base_type == BaseType.VOID and not right_is_ptr:
            self.issues.append(
                ispr.invalid_binary_operands_error(node, left_type, right_type)
            )

        # Logical AND/OR produce an int result and accept scalar operands (including pointers)
        if node.operator in ("&&", "||"):
            node.inferred_type = TypeNode(node.line, node.column, BaseType.INT)
            return

        if node.operator in ("+", "-"):
            # Case: Pointer and Pointer
            if left_is_ptr and right_is_ptr:
                if node.operator == "+":
                    # Invalid: ptr + ptr
                    self.issues.append(
                        ispr.invalid_binary_operands_error(node, left_type, right_type)
                    )
                    node.inferred_type = left_type
                else:
                    # Valid: ptr - ptr (yields ptrdiff_t/int)
                    if (
                        left_type.base_type != right_type.base_type
                        or left_type.ptr_depth != right_type.ptr_depth
                    ):
                        self.issues.append(
                            ispr.invalid_binary_operands_error(
                                node, left_type, right_type
                            )
                        )
                    node.inferred_type = TypeNode(node.line, node.column, BaseType.INT)
                return

            if left_is_ptr or right_is_ptr:
                ptr_side = left_type if left_is_ptr else right_type
                num_side = right_type if left_is_ptr else left_type

                if num_side.base_type != BaseType.INT:
                    self.issues.append(
                        ispr.invalid_binary_operands_error(node, left_type, right_type)
                    )

                if node.operator == "-" and right_is_ptr and not left_is_ptr:
                    self.issues.append(
                        ispr.invalid_binary_operands_error(node, left_type, right_type)
                    )

                node.inferred_type = ptr_side
                return

        if node.operator in ("*", "/"):
            if left_is_ptr or right_is_ptr:
                self.issues.append(
                    ispr.invalid_binary_operands_error(node, left_type, right_type)
                )
                node.inferred_type = left_type
                return

        if node.operator in ("<<", ">>", "&", "|", "^", "%"):
            if (
                left_type.base_type != BaseType.INT
                or right_type.base_type != BaseType.INT
                or left_is_ptr
                or right_is_ptr
            ):
                self.issues.append(
                    ispr.invalid_binary_operands_error(node, left_type, right_type)
                )
            node.inferred_type = TypeNode(node.line, node.column, BaseType.INT)
            return

        if (
            left_type.base_type == BaseType.FLOAT
            or right_type.base_type == BaseType.FLOAT
        ):
            node.inferred_type = TypeNode(node.line, node.column, BaseType.FLOAT)
        else:
            node.inferred_type = left_type

    def visit_FunctionCallNode(self, node: FunctionCallNode):
        sym = None

        # Check if the function name actually exists and is callable
        if isinstance(node.function, VariableNode):
            func_name = node.function.name

            # Collect arguments first to know their types for mangling
            for arg in node.arguments:
                _ = yield arg

            arg_types = []
            for arg in node.arguments:
                arg_type = getattr(arg, "inferred_type", None)
                if arg_type:
                    arg_types.append(arg_type)
                else:
                    arg_types.append(TypeNode(node.line, node.column, BaseType.INT))

            mangled_attempt = self._mangle_function(func_name, arg_types)
            sym = self.symtab.lookup(mangled_attempt)

            if sym is None:
                # Try original name (for builtins or non-overloaded functions)
                sym = self.symtab.lookup(func_name)

            if sym is None:
                all_funcs = [
                    s
                    for s in self.symtab.lookup_by_original_name(func_name)
                    if s.kind == "FUNC"
                ]
                if len(all_funcs) == 1:
                    target = all_funcs[0]
                    if len(target.params) == len(arg_types) or target.is_vararg:
                        sym = target

            if sym is None:
                sym = Symbol(
                    name=mangled_attempt,
                    original_name=func_name,
                    kind="FUNC",
                    type_info=TypeNode(node.line, node.column, BaseType.INT),
                    params=arg_types,
                    is_definition=False,
                    is_vararg=True,
                    is_implicit=True,
                )
                self.symtab.global_scope.define(sym)
                node.inferred_type = sym.type_info
                node.mangled_name = mangled_attempt
                self.called_functions.append((node, mangled_attempt))
                self.issues.append(ispr.implicit_declaration_warning(node, func_name))
            elif sym.kind != "FUNC":
                self.issues.append(
                    ispr.type_mismatch_error(node, "function", sym.kind.lower())
                )
            else:
                node.inferred_type = sym.type_info
                node.mangled_name = sym.name
                self.called_functions.append((node, sym.name))
                if not sym.is_vararg and len(node.arguments) != len(sym.params):
                    self.issues.append(
                        ispr.wrong_argument_count_error(
                            node,
                            func_name,
                            len(sym.params),
                            len(node.arguments),
                        )
                    )

        # for arg in node.arguments:  # Already yielded above
        #    _ = yield arg

        if sym and sym.kind == "FUNC":
            for i, arg in enumerate(node.arguments):
                if i >= len(sym.params):
                    break

                arg_type = getattr(arg, "inferred_type", None)
                if arg_type is None:
                    continue

                param_type = sym.params[i]
                arg_is_void = (
                    arg_type.base_type == BaseType.VOID
                    and getattr(arg_type, "ptr_depth", 0) == 0
                )
                if arg_is_void:
                    self.issues.append(
                        ispr.pasing_void_to_parameter_error(node, param_type)
                    )
                    continue

    def visit_ReturnNode(self, node):
        if node.expression:
            _ = yield node.expression

        if self.current_function is None:
            return

        fn_ret = self.current_function.return_type
        fn_is_void = (
            getattr(fn_ret, "base_type", None) == BaseType.VOID
            and getattr(fn_ret, "ptr_depth", 0) == 0
        )

        if node.expression:
            expr_type = getattr(node.expression, "inferred_type", None)

            if expr_type is None:
                return

            expr_is_void = (
                getattr(expr_type, "base_type", None) == BaseType.VOID
                and getattr(expr_type, "ptr_depth", 0) == 0
            )

            if expr_is_void and not fn_is_void:
                self.issues.append(
                    ispr.assigning_void_to_variable_error(
                        node, self.current_function.name
                    )
                )
                return

            if fn_is_void and not expr_is_void:
                self.issues.append(
                    ispr.void_should_not_return_value_warning(
                        node, self.current_function.name
                    )
                )
                pass

            fn_depth = getattr(fn_ret, "ptr_depth", 0)
            expr_depth = getattr(expr_type, "ptr_depth", 0)
            fn_base = getattr(fn_ret, "base_type", None)
            expr_base = getattr(expr_type, "base_type", None)

            if fn_base == BaseType.STRUCT or expr_base == BaseType.STRUCT:
                fn_struct = getattr(fn_ret, "struct_name", None)
                expr_struct = getattr(expr_type, "struct_name", None)

                if (
                    (fn_base != expr_base)
                    or (fn_struct != expr_struct)
                    or (fn_depth != expr_depth)
                ):
                    self.issues.append(
                        ispr.return_type_mismatch_error(node, fn_ret, expr_type)
                    )
                    return

            if fn_depth == 0 and expr_depth > 0:
                self.issues.append(
                    ispr.return_type_mismatch_error(node, fn_ret, expr_type)
                )

        else:
            if not fn_is_void:
                self.issues.append(
                    ispr.return_type_mismatch_error(node, fn_ret, "void")
                )

    def visit_WhileLoopNode(self, node: WhileLoopNode):
        if node.condition:
            _ = yield node.condition

        self.loop_depth += 1

        if node.body_block:
            _ = yield node.body_block

        self.loop_depth -= 1

        return node

    def visit_LiteralNode(self, node: LiteralNode):
        node.inferred_type = node.datatype

    def visit_CastNode(self, node: CastNode):
        resolved_target, _ = self._resolve_type_node(node.target_type, node)
        if isinstance(resolved_target, TypeNode):
            node.target_type = resolved_target

        _ = yield node.expression
        node.inferred_type = node.target_type

        # Disallow casting to/from struct/union by value
        expr_type = getattr(node.expression, "inferred_type", None)
        if expr_type is not None:
            if (
                node.target_type.base_type in (BaseType.STRUCT, BaseType.UNION)
                and node.target_type.ptr_depth == 0
            ):
                self.issues.append(
                    ispr.type_mismatch_error(node, "scalar type", "struct/union")
                )
            if (
                expr_type.base_type in (BaseType.STRUCT, BaseType.UNION)
                and expr_type.ptr_depth == 0
            ):
                self.issues.append(
                    ispr.type_mismatch_error(node, "scalar type", "struct/union")
                )

    def visit_ArrayAccessNode(self, node: ArrayAccessNode):
        _ = yield node.array
        _ = yield node.index

        arr_type = getattr(node.array, "inferred_type", None)
        idx_type = getattr(node.index, "inferred_type", None)

        if idx_type is not None:
            if (
                idx_type.base_type != BaseType.INT
                or getattr(idx_type, "ptr_depth", 0) > 0
            ):
                self.issues.append(
                    ispr.type_mismatch_error(node.index, "integer", idx_type)
                )

        if arr_type is not None:
            current_dims = getattr(arr_type, "array_dimensions", None) or []
            if not current_dims and getattr(arr_type, "ptr_depth", 0) == 0:
                self.issues.append(
                    ispr.type_mismatch_error(node.array, "array or pointer", arr_type)
                )
                node.inferred_type = arr_type
                return

            if current_dims:
                # If it's an array, index access consumes a dimension, ptr_depth stays same
                new_dims = current_dims[1:] if len(current_dims) > 1 else None
                new_ptr_depth = arr_type.ptr_depth
            else:
                # If it's a pointer being used as an array, reduce ptr_depth
                new_dims = None
                new_ptr_depth = max(0, arr_type.ptr_depth - 1)

            node.inferred_type = TypeNode(
                node.line,
                node.column,
                arr_type.base_type,
                is_const=arr_type.is_const,
                ptr_depth=new_ptr_depth,
                ptr_const_quals=(
                    list(getattr(arr_type, "ptr_const_quals", []))
                    if current_dims
                    else list(getattr(arr_type, "ptr_const_quals", []))[:-1]
                ),
                struct_name=getattr(arr_type, "struct_name", None),
            )
            node.inferred_type.array_dimensions = new_dims

    def visit_MemberAccessNode(self, node: MemberAccessNode):
        _ = yield node.object

        obj_type = getattr(node.object, "inferred_type", None)
        if obj_type is None:
            return

        if getattr(obj_type, "base_type", None) not in (
            BaseType.STRUCT,
            BaseType.UNION,
        ):
            expected = "pointer to struct/union" if node.pointer else "struct/union"
            self.issues.append(ispr.type_mismatch_error(node, expected, obj_type))
            return

        ptr_depth = getattr(obj_type, "ptr_depth", 0)
        if node.pointer:
            if ptr_depth != 1:
                self.issues.append(
                    ispr.type_mismatch_error(node, "pointer to struct", obj_type)
                )
                return
        else:
            if ptr_depth != 0:
                self.issues.append(ispr.type_mismatch_error(node, "struct", obj_type))
                return

        struct_name = getattr(obj_type, "struct_name", None)
        if not struct_name:
            self.issues.append(
                ispr.type_mismatch_error(node, "named struct type", obj_type)
            )
            return

        struct_sym = self.symtab.lookup_tag(struct_name)
        if struct_sym is None or struct_sym.kind != "STRUCT":
            self.issues.append(
                ispr.type_mismatch_error(
                    node, f"defined struct '{struct_name}'", "undefined struct"
                )
            )
            return

        field_map = getattr(struct_sym, "value", None)
        if not isinstance(field_map, dict) or node.field not in field_map:
            self.issues.append(
                ispr.type_mismatch_error(
                    node, f"existing field in struct '{struct_name}'", node.field
                )
            )
            return

        resolved_field_type, _ = self._resolve_type_node(field_map[node.field], node)
        node.inferred_type = resolved_field_type

    def visit_BreakNode(self, node: BreakNode):
        if self.loop_depth == 0:
            self.issues.append(ispr.break_not_in_loop_error(node))

    def visit_ContinueNode(self, node: ContinueNode):
        if self.loop_depth == 0:
            self.issues.append(ispr.continue_not_in_loop_error(node))

    def visit_StructNode(self, node: StructNode):
        if node.name is None:
            return

        is_def = len(node.members) > 0
        field_map = {}
        for member in node.members:
            resolved_member_type, _ = self._resolve_type_node(member.datatype, member)
            if isinstance(resolved_member_type, TypeNode):
                member.datatype = resolved_member_type

            # Prevent incomplete types/infinite recursion structs by value
            if (
                member.datatype.base_type in (BaseType.STRUCT, BaseType.UNION)
                and member.datatype.ptr_depth == 0
            ):
                m_name = member.datatype.struct_name
                if m_name:
                    m_sym = self.symtab.lookup_tag(m_name)
                    if m_sym is None or not getattr(m_sym, "is_definition", False):
                        self.issues.append(
                            ispr.type_mismatch_error(
                                member,
                                "fully defined struct/union",
                                f"incomplete type '{m_name}'",
                            )
                        )

            field_map[member.name] = member.datatype
        # Struct tags are scope-bound: a tag in an inner scope may hide an outer tag.
        # Only treat existing tags from the CURRENT scope as redeclarations/completions.
        sym = self.symtab.current_scope.symbols.get(node.name)
        if sym is not None:
            if sym.kind != "STRUCT" or sym.is_definition and is_def:
                self.issues.append(ispr.already_declared_error(node, node.name))
                return
            elif sym.kind == "STRUCT" and not sym.is_definition and is_def:
                sym.is_definition = True
                sym.value = field_map
                return
            elif sym.kind == "STRUCT" and not sym.is_definition and not is_def:
                return

        base_type = (
            BaseType.UNION if getattr(node, "is_union", False) else BaseType.STRUCT
        )

        struct_type = TypeNode(node.line, node.column, base_type, struct_name=node.name)
        struct_symbol = Symbol(
            name=node.name,
            kind="STRUCT",
            type_info=struct_type,
            value=field_map,
            is_definition=is_def,
        )
        self.symtab.define(struct_symbol)

    def visit_TypedefNode(self, node: TypedefNode):
        if not node.name or node.target_type is None:
            return

        resolved_target, ok = self._resolve_type_node(node.target_type, node)
        if not ok or not isinstance(resolved_target, TypeNode):
            return

        node.target_type = resolved_target
        existing_sym = self.symtab.current_scope.symbols.get(node.name)

        if existing_sym is not None:
            if (
                existing_sym.kind == "TYPEDEF"
                and existing_sym.type_info == node.target_type
            ):
                self.issues.append(
                    ispr.typedef_already_declared_warning(node, node.name)
                )
                return
            else:
                self.issues.append(ispr.already_declared_error(node, node.name))
                return

        typedef_symbol = Symbol(
            name=node.name,
            kind="TYPEDEF",
            type_info=node.target_type,
            is_definition=True,
        )
        self.symtab.define(typedef_symbol)

    def visit_SizeOfNode(self, node: SizeOfNode):
        if not node.is_type:
            if isinstance(node.target, VariableNode):
                sym = self.symtab.lookup(node.target.name)
                if sym and sym.kind == "TYPEDEF":
                    node.is_type = True
                    node.target = TypeNode(
                        node.line,
                        node.column,
                        BaseType.INT,
                        alias_name=node.target.name,
                    )

        if node.is_type:
            resolved_type, _ = self._resolve_type_node(node.target, node)
            if isinstance(resolved_type, TypeNode):
                node.target = resolved_type
        else:
            _ = yield node.target
            # Specific exception for sizeof: arrays do not decay to pointers
            if isinstance(node.target, VariableNode):
                sym = self.symtab.lookup(node.target.name)
                if sym and getattr(sym.type_info, "array_dimensions", None):
                    node.target.inferred_type = sym.type_info

        node.inferred_type = TypeNode(node.line, node.column, BaseType.INT)
