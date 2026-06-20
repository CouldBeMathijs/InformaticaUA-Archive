from src.parser.AST.Node import *
from src.parser.grammars.GrammarParser import GrammarParser
from src.parser.grammars.GrammarVisitor import GrammarVisitor
import codecs
import src.issue_printer as ispr
from copy import deepcopy


class Builder(GrammarVisitor):
    def __init__(
        self, stream, allow_declarations_in_for_loops: bool, issue_collector=None
    ):
        self.token_stream = stream
        self.issues = issue_collector if issue_collector is not None else []
        self.allow_declarations_in_for_loops = allow_declarations_in_for_loops

    #   --- Helper methods ---

    def _flatten_statements(self, statements_ctx):
        statements = []
        for s in statements_ctx:
            if s:
                res = self.visit(s)
                if isinstance(res, list):
                    statements.extend(res)
                elif res:
                    statements.append(res)
        return statements

    def _extract_comments(self, ctx):
        comments = []
        hidden_tokens = self.token_stream.getHiddenTokensToRight(ctx.stop.tokenIndex, 1)
        if not hidden_tokens:
            return comments

        for token in hidden_tokens:
            comment = token.text
            snipped_comment = comment[2:-2]
            if "/*" in snipped_comment:
                dummy = Node(token.line, token.column)
                self.issues.append(ispr.block_comment_within_block_comment_error(dummy))

            comments.append(comment)
        return comments

    def _unescape_c_style(self, text):
        content = text[1:-1]
        return codecs.decode(content, "unicode_escape")

    def visitRoot(self, ctx: GrammarParser.RootContext):
        header_elements = self._flatten_statements(ctx.headerElement())
        program_node = ProgramNode(
            ctx.start.line,
            ctx.start.column,
            header_elements=header_elements,
        )
        if not program_node.main_function:
            self.issues.append(ispr.missing_main_error(program_node))
        return program_node

    def visitFunction(self, ctx: GrammarParser.FunctionContext):
        if ctx.parameters():
            parameter_types = self.visit(ctx.parameters())
        else:
            parameter_types = []

        if ctx.getText().endswith(";"):
            statements = None
        else:
            statements = self._flatten_statements(ctx.statement())
        if ctx.type_() is not None:
            return_type = self.visit(ctx.type_())
        else:
            return_type = TypeNode(ctx.start.line, ctx.start.column, BaseType.INT)
        name = ctx.ID().getText()
        if name == "main" and parameter_types == []:
            if not return_type.is_basic(BaseType.INT):
                dummy = Node(ctx.start.line, ctx.start.column)
                self.issues.append(ispr.return_type_main_not_int(dummy, return_type))
            return MainFunctionNode(ctx.start.line, ctx.start.column, statements)
        else:
            return FunctionNode(
                ctx.start.line,
                ctx.start.column,
                name,
                parameter_types,
                statements,
                return_type,
            )

    def visitParameters(self, ctx: GrammarParser.ParametersContext):
        params = [self.visit(p) for p in ctx.parameter() if p]
        return params

    def visitParameter(self, ctx: GrammarParser.ParameterContext):
        datatype = self.visit(ctx.type_())
        if ctx.array_sizes() is not None:
            datatype.ptr_depth += 1
            datatype.array_dimensions = None
        name = ctx.ID().getText()
        return ParameterNode(ctx.start.line, ctx.start.column, datatype, name)

    def visitFunctionStmt(self, ctx: GrammarParser.FunctionStmtContext):
        return self.visit(ctx.function())

    # --- UPGRADED: Handlers for Enums ---

    def visitEnum(self, ctx: GrammarParser.EnumContext):
        name = ctx.ID().getText() if ctx.ID() else None
        labels = self.visit(ctx.labels())

        # Inject the enum_name context into all parsed LiteralNodes (labels)
        for lbl in labels:
            if isinstance(lbl, LiteralNode):
                lbl.enum_name = name

        return EnumNode(ctx.start.line, ctx.start.column, name, labels)

    def visitLabels(self, ctx: GrammarParser.LabelsContext):
        return [self.visit(lbl) for lbl in ctx.label() if lbl]

    def visitLabel(self, ctx: GrammarParser.LabelContext):
        name = ctx.ID().getText()
        value = self.visit(ctx.expr()) if ctx.expr() else None

        # Map enum labels directly to LiteralNodes as ints
        return LiteralNode(
            line=ctx.start.line,
            column=ctx.start.column,
            value=value,
            datatype=TypeNode(ctx.start.line, ctx.start.column, BaseType.INT),
            enum_label=name,
            comments=self._extract_comments(ctx),
        )

    def visitSimpleStmt(self, ctx: GrammarParser.SimpleStmtContext):
        return self.visitStatement(ctx)

    def visitEmptyStmt(self, ctx: GrammarParser.EmptyStmtContext):
        return self.visitStatement(ctx)

    def visitEnumStmt(self, ctx: GrammarParser.EnumStmtContext):
        return self.visitStatement(ctx)

    def visitStructStmt(self, ctx: GrammarParser.StructStmtContext):
        return self.visitStatement(ctx)

    def visitIfStmt(self, ctx: GrammarParser.IfStmtContext):
        return self.visitStatement(ctx)

    def visitSwitchStmt(self, ctx: GrammarParser.SwitchStmtContext):
        return self.visitStatement(ctx)

    def visitTypedefStmt(self, ctx: GrammarParser.TypedefStmtContext):
        return self.visitStatement(ctx)

    def visitStatement(self, ctx: GrammarParser.StatementContext):
        is_simple = isinstance(ctx, GrammarParser.SimpleStmtContext)
        is_empty = isinstance(ctx, GrammarParser.EmptyStmtContext)
        is_enum = isinstance(ctx, GrammarParser.EnumStmtContext)
        is_struct = isinstance(ctx, GrammarParser.StructStmtContext)
        is_switch = isinstance(ctx, GrammarParser.SwitchStmtContext)
        is_break = isinstance(ctx, GrammarParser.BreakStmtContext)
        is_continue = isinstance(ctx, GrammarParser.ContinueStmtContext)
        is_typedef = isinstance(ctx, GrammarParser.TypedefStmtContext)

        is_if = isinstance(ctx, GrammarParser.IfStmtContext)
        is_anon = isinstance(ctx, GrammarParser.AnonymousScopeContext)
        is_func = isinstance(ctx, GrammarParser.FunctionStmtContext)
        is_while = isinstance(ctx, GrammarParser.FunctionStmtContext)
        is_for = isinstance(ctx, GrammarParser.FunctionStmtContext)
        is_return = isinstance(ctx, GrammarParser.FunctionStmtContext)

        if not (
            is_simple
            or is_func
            or is_enum
            or is_struct
            or is_typedef
            or is_anon
            or is_if
            or is_while
            or is_for
            or is_switch
            or is_break
            or is_continue
            or is_return
            or is_empty
        ):
            feature_name = type(ctx).__name__.replace("Context", "")
            dummy = Node(ctx.start.line, ctx.start.column)
            self.issues.append(ispr.missing_feature_error(dummy, feature_name))
            return None

        node = self.visit(ctx.getChild(0))
        if node is not None:
            if isinstance(node, list):
                for n in node:
                    n.comments = self._extract_comments(ctx)
            else:
                node.comments = self._extract_comments(ctx)
        return node

    def visitSwitchStatement(self, ctx: GrammarParser.SwitchStatementContext):
        condition = self.visit(ctx.expr())
        cases = list(ctx.switchCase())

        parsed_cases = []

        for case_ctx in cases:
            is_default = case_ctx.getText().startswith("default")
            case_expr = None if is_default else self.visit(case_ctx.expr())

            stmts = []
            has_break = False

            if case_ctx.statement():
                for s in case_ctx.statement():
                    stmt_node = self.visit(s)

                    if isinstance(stmt_node, DeclarationNode):
                        self.issues.append(
                            ispr.declaration_within_switch_error(stmt_node)
                        )
                        continue

                    if isinstance(stmt_node, BreakNode):
                        has_break = True
                        break

                    if stmt_node:
                        stmts.append(stmt_node)

            parsed_cases.append(
                {
                    "is_default": is_default,
                    "expr": case_expr,
                    "stmts": stmts,
                    "has_break": has_break,
                    "line": case_ctx.start.line,
                    "col": case_ctx.start.column,
                }
            )

        effective_stmts_per_case = []
        current_fallthrough = []

        for c in reversed(parsed_cases):
            if c["has_break"]:
                current_fallthrough = []
            combined_stmts = deepcopy(c["stmts"]) + deepcopy(current_fallthrough)
            effective_stmts_per_case.append(combined_stmts)
            current_fallthrough = combined_stmts

        effective_stmts_per_case.reverse()

        grouped_cases = []
        current_conditions = []

        for i, c in enumerate(parsed_cases):
            if not c["is_default"]:
                cond = BinaryOpNode(c["line"], c["col"], condition, "==", c["expr"])
                current_conditions.append(cond)
            else:
                current_conditions.append("default")

            if (
                len(c["stmts"]) == 0
                and not c["has_break"]
                and i < len(parsed_cases) - 1
            ):
                pass
            else:
                final_cond = None
                has_default = False

                for cd in current_conditions:
                    if cd == "default":
                        has_default = True
                    elif final_cond is None:
                        final_cond = cd
                    else:
                        final_cond = BinaryOpNode(
                            c["line"], c["col"], final_cond, "||", cd
                        )

                grouped_cases.append(
                    (has_default, final_cond, effective_stmts_per_case[i])
                )
                current_conditions = []

        default_stmts = None
        normal_cases = []

        for is_default, cond, stmts in grouped_cases:
            if is_default:
                default_stmts = stmts
            else:
                normal_cases.append((cond, stmts))

        current_else_block = None
        if default_stmts is not None:
            current_else_block = BlockNode(
                ctx.start.line, ctx.start.column, default_stmts
            )

        for cond, stmts in reversed(normal_cases):
            block_node = BlockNode(ctx.start.line, ctx.start.column, stmts)
            new_if = IfNode(
                ctx.start.line, ctx.start.column, cond, block_node, current_else_block
            )
            current_else_block = new_if

        if current_else_block is None:
            return BlockNode(ctx.start.line, ctx.start.column, [])

        return current_else_block

    def visitAnonymousScope(self, ctx: GrammarParser.AnonymousScopeContext):
        statements = self._flatten_statements(ctx.statement())
        return BlockNode(ctx.start.line, ctx.start.column, statements)

    def visitIfStatement(self, ctx: GrammarParser.IfStatementContext):
        condition = self.visit(ctx.expr())
        if_block = self.visit(ctx.statement(0))
        if isinstance(if_block, list):
            if_block = BlockNode(ctx.start.line, ctx.start.column, if_block)

        else_block = None
        if ctx.statement(1):
            else_block = self.visit(ctx.statement(1))
            if isinstance(else_block, list):
                else_block = BlockNode(ctx.start.line, ctx.start.column, else_block)

        return IfNode(ctx.start.line, ctx.start.column, condition, if_block, else_block)

    def visitInclude(self, ctx: GrammarParser.IncludeContext):
        return self.visit(ctx.header())

    def visitHeader(self, ctx: GrammarParser.HeaderContext):
        if ctx.HEADER_SYSTEM:
            file = ctx.HEADER_SYSTEM().getText()
            file = file[1 : len(file) - 1]
            node = IncludeNode(ctx.start.line, ctx.start.column, file)
            node.system = True
        else:
            file = ctx.HEADER_LOCAL().getText()
            file = file[1 : len(file) - 1]
            node = IncludeNode(ctx.start.line, ctx.start.column, file)

        return node

    def visitBreakStmt(self, ctx: GrammarParser.BreakStmtContext):
        node = BreakNode(ctx.start.line, ctx.start.column)
        node.comments = self._extract_comments(ctx)
        return node

    def visitContinueStmt(self, ctx: GrammarParser.ContinueStmtContext):
        node = ContinueNode(ctx.start.line, ctx.start.column)
        node.comments = self._extract_comments(ctx)
        return node

    # --- Expression Handlers ---

    def visitPrimary_paren(self, ctx: GrammarParser.Primary_parenContext):
        return self.visit(ctx.expr())

    def visitPrimary_id(self, ctx: GrammarParser.Primary_idContext):
        return VariableNode(ctx.start.line, ctx.start.column, ctx.ID().getText())

    def visitPrimary_lit(self, ctx: GrammarParser.Primary_litContext):
        return self.visit(ctx.literal())

    def visitAssignment(self, ctx: GrammarParser.AssignmentContext):
        target = self.visit(ctx.expr(0))
        value = self.visit(ctx.expr(1))
        return AssignmentNode(ctx.start.line, ctx.start.column, target, value)

    def visitFunction_call(self, ctx: GrammarParser.Function_callContext):
        func_expr = self.visit(ctx.expr())
        args = [self.visit(e) for e in ctx.arg_list().expr()] if ctx.arg_list() else []
        return FunctionCallNode(ctx.start.line, ctx.start.column, func_expr, args)

    def visitArray_access(self, ctx: GrammarParser.Array_accessContext):
        array = self.visit(ctx.expr(0))
        index = self.visit(ctx.expr(1))
        return ArrayAccessNode(ctx.start.line, ctx.start.column, array, index)

    def visitInitializer_list(self, ctx: GrammarParser.Initializer_listContext):
        elements = []
        for e in ctx.initializer_element():
            elements.append(self.visit(e))
        return InitializerListNode(ctx.start.line, ctx.start.column, elements)

    def visitInitializer_element(self, ctx: GrammarParser.Initializer_elementContext):
        return self.visit(ctx.getChild(0))

    def visitBinary(self, ctx: GrammarParser.BinaryContext):
        left = self.visit(ctx.expr(0))
        right = self.visit(ctx.expr(1))
        operator = ctx.getChild(1).getText()
        return BinaryOpNode(ctx.start.line, ctx.start.column, left, operator, right)

    def visitUnary_cast(self, ctx: GrammarParser.Unary_castContext):
        target_type = self.visit(ctx.type_())
        expression = self.visit(ctx.expr())
        return CastNode(ctx.start.line, ctx.start.column, target_type, expression)

    def visitUnary_prefix(self, ctx: GrammarParser.Unary_prefixContext):
        operator = ctx.getChild(0).getText()
        operand = self.visit(ctx.expr())
        return UnaryOpNode(
            ctx.start.line, ctx.start.column, operator, operand, postfix=False
        )

    def visitUnary_postfix(self, ctx: GrammarParser.Unary_postfixContext):
        operand = self.visit(ctx.expr())
        operator = ctx.getChild(1).getText()
        return UnaryOpNode(
            ctx.start.line, ctx.start.column, operator, operand, postfix=True
        )

    # --- Declarations & Types ---

    def visitArray_sizes(self, ctx: GrammarParser.Array_sizesContext):
        sizes = []
        children = list(ctx.getChildren())

        for i in range(len(children)):
            if children[i].getText() == "[":
                next_child = children[i + 1]

                if next_child.getText() == "]":
                    sizes.append(None)
                else:
                    sizes.append(self.visit(next_child))
        return sizes

    def visitDeclaration(self, ctx: GrammarParser.DeclarationContext):
        base_datatype = self.visit(ctx.type_())
        declarations = []

        for init_decl_ctx in ctx.init_declarator():
            datatype = deepcopy(base_datatype)
            name = init_decl_ctx.ID().getText()
            datatype.array_dimensions = (
                self.visit(init_decl_ctx.array_sizes())
                if init_decl_ctx.array_sizes()
                else None
            )

            init = None
            if init_decl_ctx.expr():
                init = self.visit(init_decl_ctx.expr())
            elif init_decl_ctx.initializer_list():
                init = self.visit(init_decl_ctx.initializer_list())

            decl_node = DeclarationNode(
                init_decl_ctx.ID().getSymbol().line,
                init_decl_ctx.ID().getSymbol().column,
                datatype,
                name,
                init,
            )
            declarations.append(decl_node)

        return declarations

    def visitType(self, ctx: GrammarParser.TypeContext):
        ts_ctx = ctx.typeSpecifier()
        base_const = len(ctx.typeQualifier()) > 0
        pointer_quals = [len(pq.typeQualifier()) > 0 for pq in ctx.pointerQualifier()]
        ptr_depth = len(pointer_quals)

        # Fallback for malformed "const"-only types.
        if ts_ctx is None:
            return TypeNode(
                ctx.start.line,
                ctx.start.column,
                BaseType.INT,
                base_const,
                ptr_depth,
                ptr_const_quals=pointer_quals,
            )

        spec_info = self.visit(ts_ctx)
        base_type = spec_info.get("base_type", BaseType.VOID)
        struct_name = spec_info.get("struct_name")
        alias_name = spec_info.get("alias_name")

        return TypeNode(
            ctx.start.line,
            ctx.start.column,
            base_type,
            base_const,
            ptr_depth,
            ptr_const_quals=pointer_quals,
            struct_name=struct_name,
            alias_name=alias_name,
        )

    def visitBaseSpecifier(self, ctx: GrammarParser.BaseSpecifierContext):
        base = ctx.keyword().getText()
        mapping = {
            "char": BaseType.CHAR,
            "int": BaseType.INT,
            "float": BaseType.FLOAT,
            "void": BaseType.VOID,
        }
        return {"base_type": mapping.get(base, BaseType.VOID)}

    def visitEnumSpecifier(self, ctx: GrammarParser.EnumSpecifierContext):
        # In C, enums are represented as integers.
        return {"base_type": BaseType.INT}

    def visitStructSpecifier(self, ctx: GrammarParser.StructSpecifierContext):
        is_union = ctx.getChild(0).getText() == "union"
        base = BaseType.UNION if is_union else BaseType.STRUCT
        return {"base_type": base, "struct_name": ctx.ID().getText()}

    def visitTypedefSpecifier(self, ctx: GrammarParser.TypedefSpecifierContext):
        # Keep alias information for semantic resolution in the symbol table phase.
        return {"base_type": BaseType.INT, "alias_name": ctx.ID().getText()}

    def visitLiteral(self, ctx: GrammarParser.LiteralContext):
        t = ctx.start
        text = ctx.getText()

        if ctx.INT():
            return LiteralNode(
                t.line,
                t.column,
                int(text, 0),
                TypeNode(t.line, t.column, BaseType.INT),
            )

        if ctx.REAL():
            return LiteralNode(
                t.line,
                t.column,
                float(text),
                TypeNode(t.line, t.column, BaseType.FLOAT),
            )

        if ctx.CHAR():
            unescaped = self._unescape_c_style(text)
            if len(unescaped) > 1:
                self.issues.append(ispr.multi_character_char_warning(ctx.start))
                unescaped = unescaped[-1]
            val = ord(unescaped)
            return LiteralNode(
                t.line,
                t.column,
                val,
                TypeNode(t.line, t.column, BaseType.CHAR),
            )

        if ctx.STRING():
            unescaped = self._unescape_c_style(text)
            return LiteralNode(
                t.line,
                t.column,
                unescaped,
                TypeNode(t.line, t.column, BaseType.CHAR, ptr_depth=1),
            )
        return None

    def visitReturnStmt(self, ctx: GrammarParser.ReturnStmtContext):
        expr = self.visit(ctx.expr()) if ctx.expr() else None
        node = ReturnNode(ctx.start.line, ctx.start.column, expr)
        node.comments = self._extract_comments(ctx)
        return node

    def visitForInit(self, ctx: GrammarParser.ForInitContext):
        if self.allow_declarations_in_for_loops:
            return self.visit(ctx.getChild(0))
        else:
            if ctx.expr():
                return self.visit(ctx.expr())
            else:
                self.issues.append(ispr.declaration_in_for_loop_error(ctx.start))

    def visitForLoop(self, ctx: GrammarParser.ForLoopContext):
        init_node = self.visit(ctx.init) if ctx.init else None

        if ctx.cond:
            cond_node = self.visit(ctx.cond)
        else:
            cond_node = LiteralNode(
                ctx.start.line,
                ctx.start.column,
                1,
                TypeNode(ctx.start.line, ctx.start.column, BaseType.INT),
            )

        update_node = self.visit(ctx.update) if ctx.update else None
        body_node = self.visit(ctx.body)

        line, col = ctx.start.line, ctx.start.column
        first_var = f"__first_{line}_{col}"
        int_type = TypeNode(line, col, BaseType.INT)

        first_decl = DeclarationNode(
            line, col, int_type, first_var, LiteralNode(line, col, 1, int_type)
        )

        update_stmt = update_node
        if isinstance(update_stmt, list):
            update_stmt = BlockNode(line, col, update_stmt)

        if_not_first = IfNode(
            line,
            col,
            UnaryOpNode(line, col, "!", VariableNode(line, col, first_var)),
            update_stmt if update_stmt else BlockNode(line, col, []),
        )

        set_first_zero = AssignmentNode(
            line,
            col,
            VariableNode(line, col, first_var),
            LiteralNode(line, col, 0, int_type),
        )

        if_not_cond_break = IfNode(
            line, col, UnaryOpNode(line, col, "!", cond_node), BreakNode(line, col)
        )

        while_body_stmts = [if_not_first, set_first_zero, if_not_cond_break]
        if isinstance(body_node, BlockNode):
            while_body_stmts.extend(body_node.statements)
        elif body_node:
            while_body_stmts.append(body_node)

        while_body = BlockNode(line, col, while_body_stmts)

        while_node = WhileLoopNode(
            line, col, LiteralNode(line, col, 1, int_type), while_body
        )
        while_node.comments = self._extract_comments(ctx)

        final_stmts = []
        if init_node:
            if isinstance(init_node, list):
                final_stmts.extend(init_node)
            else:
                final_stmts.append(init_node)
        final_stmts.append(first_decl)
        final_stmts.append(while_node)

        return BlockNode(line, col, final_stmts)

    def visitWhileLoop(self, ctx: GrammarParser.WhileLoopContext):
        condition = self.visit(ctx.expr())
        body = self.visit(ctx.statement())
        if isinstance(body, list):
            body = BlockNode(ctx.start.line, ctx.start.column, body)
        node = WhileLoopNode(ctx.start.line, ctx.start.column, condition, body)
        node.comments = self._extract_comments(ctx)
        return node

    def visitGlobalDeclaration(self, ctx: GrammarParser.GlobalDeclarationContext):
        node = self.visit(ctx.declaration())
        if node is not None:
            if isinstance(node, list):
                for n in node:
                    n.comments = self._extract_comments(ctx)
            else:
                node.comments = self._extract_comments(ctx)
        return node

    def visitStructDecl(self, ctx: GrammarParser.StructDeclContext):
        # Controleer het allereerste token om te zien of het een union is
        is_union = ctx.getChild(0).getText() == "union"
        name = ctx.ID().getText() if ctx.ID() else None

        members = []
        for field_decl in ctx.structFieldDecl():
            members.extend(self.visit(field_decl))

        return StructNode(
            ctx.start.line,
            ctx.start.column,
            name,
            members,
            is_union=is_union,
            comments=self._extract_comments(ctx),
        )

    def visitStructFieldDecl(self, ctx: GrammarParser.StructFieldDeclContext):
        base_type = self.visit(ctx.type_())
        fields = self.visit(ctx.structFieldList())
        out = []
        for field in fields:
            field_type = deepcopy(base_type)
            field_type.array_dimensions = field["array_dims"]
            out.append(
                StructFieldNode(
                    field["line"],
                    field["col"],
                    field["name"],
                    field_type,
                    self._extract_comments(ctx),
                )
            )
        return out

    def visitStructFieldList(self, ctx: GrammarParser.StructFieldListContext):
        return [self.visit(f) for f in ctx.structField()]

    def visitStructField(self, ctx: GrammarParser.StructFieldContext):
        return {
            "name": ctx.ID().getText(),
            "array_dims": self.visit(ctx.array_sizes()) if ctx.array_sizes() else None,
            "line": ctx.ID().getSymbol().line,
            "col": ctx.ID().getSymbol().column,
        }

    def visitMember_access(self, ctx: GrammarParser.Member_accessContext):
        return MemberAccessNode(
            ctx.start.line,
            ctx.start.column,
            self.visit(ctx.expr()),
            ctx.ID().getText(),
            comments=self._extract_comments(ctx),
        )

    def visitMember_access_ptr(self, ctx: GrammarParser.Member_access_ptrContext):
        return MemberAccessNode(
            ctx.start.line,
            ctx.start.column,
            self.visit(ctx.expr()),
            ctx.ID().getText(),
            True,
            self._extract_comments(ctx),
        )

    def visitTypedefDecl(self, ctx: GrammarParser.TypedefDeclContext):
        if ctx.ID() is None:
            name = ""
            type_node = None
        else:
            name = ctx.ID().getText()
            type_node = self.visit(ctx.type_())

        return TypedefNode(
            ctx.start.line,
            ctx.start.column,
            name,
            type_node,
            self._extract_comments(ctx),
        )

    def visitSizeof_expr(self, ctx: GrammarParser.Sizeof_exprContext):
        target_expr = self.visit(ctx.expr())
        return SizeOfNode(
            ctx.start.line,
            ctx.start.column,
            target_expr,
            is_type=False,
            comments=self._extract_comments(ctx),
        )

    def visitSizeof_type(self, ctx: GrammarParser.Sizeof_typeContext):
        target_type = self.visit(ctx.type_())
        return SizeOfNode(
            ctx.start.line,
            ctx.start.column,
            target_type,
            is_type=True,
            comments=self._extract_comments(ctx),
        )
