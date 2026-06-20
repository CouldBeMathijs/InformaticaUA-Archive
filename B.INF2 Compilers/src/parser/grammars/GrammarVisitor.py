# Generated from src/parser/grammars/Grammar.g4 by ANTLR 4.13.1
from antlr4 import *
if "." in __name__:
    from .GrammarParser import GrammarParser
else:
    from GrammarParser import GrammarParser

# This class defines a complete generic visitor for a parse tree produced by GrammarParser.

class GrammarVisitor(ParseTreeVisitor):

    # Visit a parse tree produced by GrammarParser#root.
    def visitRoot(self, ctx:GrammarParser.RootContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#headerElement.
    def visitHeaderElement(self, ctx:GrammarParser.HeaderElementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#typedefDecl.
    def visitTypedefDecl(self, ctx:GrammarParser.TypedefDeclContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#structDecl.
    def visitStructDecl(self, ctx:GrammarParser.StructDeclContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#structFieldDecl.
    def visitStructFieldDecl(self, ctx:GrammarParser.StructFieldDeclContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#structFieldList.
    def visitStructFieldList(self, ctx:GrammarParser.StructFieldListContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#structField.
    def visitStructField(self, ctx:GrammarParser.StructFieldContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#enum.
    def visitEnum(self, ctx:GrammarParser.EnumContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#globalDeclaration.
    def visitGlobalDeclaration(self, ctx:GrammarParser.GlobalDeclarationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#labels.
    def visitLabels(self, ctx:GrammarParser.LabelsContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#label.
    def visitLabel(self, ctx:GrammarParser.LabelContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#include.
    def visitInclude(self, ctx:GrammarParser.IncludeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#header.
    def visitHeader(self, ctx:GrammarParser.HeaderContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#function.
    def visitFunction(self, ctx:GrammarParser.FunctionContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#parameters.
    def visitParameters(self, ctx:GrammarParser.ParametersContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#parameter.
    def visitParameter(self, ctx:GrammarParser.ParameterContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#simpleStmt.
    def visitSimpleStmt(self, ctx:GrammarParser.SimpleStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#functionStmt.
    def visitFunctionStmt(self, ctx:GrammarParser.FunctionStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#enumStmt.
    def visitEnumStmt(self, ctx:GrammarParser.EnumStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#structStmt.
    def visitStructStmt(self, ctx:GrammarParser.StructStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#typedefStmt.
    def visitTypedefStmt(self, ctx:GrammarParser.TypedefStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#anonymousScope.
    def visitAnonymousScope(self, ctx:GrammarParser.AnonymousScopeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#ifStmt.
    def visitIfStmt(self, ctx:GrammarParser.IfStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#whileStmt.
    def visitWhileStmt(self, ctx:GrammarParser.WhileStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#forStmt.
    def visitForStmt(self, ctx:GrammarParser.ForStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#switchStmt.
    def visitSwitchStmt(self, ctx:GrammarParser.SwitchStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#breakStmt.
    def visitBreakStmt(self, ctx:GrammarParser.BreakStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#continueStmt.
    def visitContinueStmt(self, ctx:GrammarParser.ContinueStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#returnStmt.
    def visitReturnStmt(self, ctx:GrammarParser.ReturnStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#emptyStmt.
    def visitEmptyStmt(self, ctx:GrammarParser.EmptyStmtContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#ifStatement.
    def visitIfStatement(self, ctx:GrammarParser.IfStatementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#whileLoop.
    def visitWhileLoop(self, ctx:GrammarParser.WhileLoopContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#forInit.
    def visitForInit(self, ctx:GrammarParser.ForInitContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#forLoop.
    def visitForLoop(self, ctx:GrammarParser.ForLoopContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#switchStatement.
    def visitSwitchStatement(self, ctx:GrammarParser.SwitchStatementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#switchCase.
    def visitSwitchCase(self, ctx:GrammarParser.SwitchCaseContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#sizeof_type.
    def visitSizeof_type(self, ctx:GrammarParser.Sizeof_typeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#function_call.
    def visitFunction_call(self, ctx:GrammarParser.Function_callContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#array_access.
    def visitArray_access(self, ctx:GrammarParser.Array_accessContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#assignment.
    def visitAssignment(self, ctx:GrammarParser.AssignmentContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#sizeof_expr.
    def visitSizeof_expr(self, ctx:GrammarParser.Sizeof_exprContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#member_access_ptr.
    def visitMember_access_ptr(self, ctx:GrammarParser.Member_access_ptrContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#unary_postfix.
    def visitUnary_postfix(self, ctx:GrammarParser.Unary_postfixContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#member_access.
    def visitMember_access(self, ctx:GrammarParser.Member_accessContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#primary_lit.
    def visitPrimary_lit(self, ctx:GrammarParser.Primary_litContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#primary_paren.
    def visitPrimary_paren(self, ctx:GrammarParser.Primary_parenContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#unary_prefix.
    def visitUnary_prefix(self, ctx:GrammarParser.Unary_prefixContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#binary.
    def visitBinary(self, ctx:GrammarParser.BinaryContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#primary_id.
    def visitPrimary_id(self, ctx:GrammarParser.Primary_idContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#unary_cast.
    def visitUnary_cast(self, ctx:GrammarParser.Unary_castContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#arg_list.
    def visitArg_list(self, ctx:GrammarParser.Arg_listContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#literal.
    def visitLiteral(self, ctx:GrammarParser.LiteralContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#declaration.
    def visitDeclaration(self, ctx:GrammarParser.DeclarationContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#init_declarator.
    def visitInit_declarator(self, ctx:GrammarParser.Init_declaratorContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#initializer_list.
    def visitInitializer_list(self, ctx:GrammarParser.Initializer_listContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#initializer_element.
    def visitInitializer_element(self, ctx:GrammarParser.Initializer_elementContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#array_sizes.
    def visitArray_sizes(self, ctx:GrammarParser.Array_sizesContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#type.
    def visitType(self, ctx:GrammarParser.TypeContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#pointerQualifier.
    def visitPointerQualifier(self, ctx:GrammarParser.PointerQualifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#typeQualifier.
    def visitTypeQualifier(self, ctx:GrammarParser.TypeQualifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#baseSpecifier.
    def visitBaseSpecifier(self, ctx:GrammarParser.BaseSpecifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#enumSpecifier.
    def visitEnumSpecifier(self, ctx:GrammarParser.EnumSpecifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#structSpecifier.
    def visitStructSpecifier(self, ctx:GrammarParser.StructSpecifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#typedefSpecifier.
    def visitTypedefSpecifier(self, ctx:GrammarParser.TypedefSpecifierContext):
        return self.visitChildren(ctx)


    # Visit a parse tree produced by GrammarParser#keyword.
    def visitKeyword(self, ctx:GrammarParser.KeywordContext):
        return self.visitChildren(ctx)



del GrammarParser