grammar Grammar;

root : (headerElement)* EOF ;

headerElement
    : include
    | enum
    | function
    | globalDeclaration
    | structDecl
    | typedefDecl
    | expr ';'
    | ';'
    ;


typedefDecl
    : 'typedef' type ID ';'
    | 'typedef;'
    ;

structDecl
     : ('struct' | 'union') ID? '{' structFieldDecl* '}' ';'
     | ('struct' | 'union') ID ';'
     ;

 structFieldDecl
     : type structFieldList ';'
     ;

 structFieldList
     : structField (',' structField)*
     ;

 structField
     : ID array_sizes?
     ;

enum
    : 'enum' ID? '{' labels '}' ';'
    ;

globalDeclaration
    : declaration ';'
    ;

labels
    : label (',' label)* ','?
    ;

label
    : ID ('=' expr)?
    ;

include
    : '#include' header
    ;

header
    : HEADER_SYSTEM
    | HEADER_LOCAL
    ;

function
    : type ID '(' parameters? ')' (';' | '{' statement* '}')
    | ID '(' parameters? ')' (';' | '{' statement* '}')
    ;

parameters
    : parameter (',' parameter)*
    ;

parameter
    : type ID array_sizes?
    ;


statement
    : (declaration | expr) ';'      # simpleStmt
    | function                      # functionStmt
    | enum                          # enumStmt
    | structDecl                    # structStmt
    | typedefDecl                   # typedefStmt
    | '{' statement* '}'            # anonymousScope
    | ifStatement                   # ifStmt
    | whileLoop                     # whileStmt
    | forLoop                       # forStmt
    | switchStatement               # switchStmt
    | 'break' ';'                   # breakStmt
    | 'continue' ';'                # continueStmt
    | 'return' expr? ';'            # returnStmt
    | ';'                           # emptyStmt
    ;


ifStatement
    : 'if' '(' expr ')' statement ('else' statement)?
    ;

whileLoop
    : 'while' '(' expr ')' statement
    ;

forInit
    : declaration
    | expr
    ;

forLoop
    : 'for' '('
      init=forInit? ';'
      cond=expr? ';'
      update=expr?
      ')' body=statement
    ;

switchStatement
    : 'switch' '(' expr ')' '{' switchCase* '}'
    ;

switchCase
    : 'case' expr ':' statement*
    | 'default' ':' statement*
    ;

expr
    : '(' expr ')'                                            # primary_paren
    | ID                                                      # primary_id
    | literal                                                 # primary_lit
    | 'sizeof' '(' expr ')'                                   # sizeof_expr
    | 'sizeof' '(' type ')'                                   # sizeof_type
    | expr '(' arg_list? ')'                                  # function_call
    | expr '[' expr ']'                                       # array_access
    | expr '.' ID                                             # member_access
    | expr '->' ID                                            # member_access_ptr
    | expr ('++' | '--')                                      # unary_postfix
    | ('(' type ')') expr                                     # unary_cast
    | ('+' | '-' | '!' | '~' | '*' | '&' | '--' | '++') expr  # unary_prefix
    | expr ('*' | '/' | '%') expr                             # binary
    | expr ('+' | '-') expr                                   # binary
    | expr ('<<' | '>>') expr                                 # binary
    | expr ('<' | '>' | '<=' | '>=') expr                     # binary
    | expr ('==' | '!=') expr                                 # binary
    | expr '&' expr                                           # binary
    | expr '^' expr                                           # binary
    | expr '|' expr                                           # binary
    | expr '&&' expr                                          # binary
    | expr '||' expr                                          # binary
    | <assoc=right> expr '=' expr                             # assignment
    ;

arg_list
    : expr (',' expr)*
    ;

literal
    : INT
    | REAL
    | CHAR
    | STRING
    ;

declaration
    : type init_declarator (',' init_declarator)*
    ;

init_declarator
    : ID array_sizes? ('=' (expr | initializer_list))?
    ;

initializer_list
    : '{' (initializer_element (',' initializer_element)*)? ','? '}'
    ;

initializer_element
    : expr
    | initializer_list
    ;

array_sizes
    : '[' ']' ('[' expr ']')*
    | ('[' expr ']')+
    ;

type
    : typeQualifier* typeSpecifier typeQualifier* pointerQualifier*
    | typeQualifier+ pointerQualifier*
    ;

pointerQualifier
    : typeQualifier* '*' typeQualifier*
    ;

typeQualifier
    : 'const'
    ;

typeSpecifier
    : keyword       # baseSpecifier
    | 'enum' ID     # enumSpecifier
    | ('struct' | 'union') ID   # structSpecifier
    | ID            # typedefSpecifier
    ;

keyword
    : 'float' | 'int' | 'char' | 'void'
    ;

CHAR : '\'' ( ~['\\\r\n] | ANSI_ESCAPE )+ '\'' ;

STRING : '"' ( ~["\\\r\n] | ANSI_ESCAPE )* '"' ;

fragment ANSI_ESCAPE
    : '\\' [abfnrtv\\'?]
    | '\\' [0-7] [0-7]? [0-7]?
    | '\\x' [0-9a-fA-F]+
    ;

INT : HEX_LITERAL
    | OCT_LITERAL
    | DEC_LITERAL
    ;

fragment DEC_LITERAL : '0' | [1-9] [0-9]* ;

fragment OCT_LITERAL : '0' [0-7]+ ;

fragment HEX_LITERAL : '0' [xX] [0-9a-fA-F]+ ;

REAL : [0-9]+ '.' [0-9]* | '.' [0-9]+ ;
ID   : [a-zA-Z_] [a-zA-Z0-9_]* ;
WS   : [ \t\n\r]+ -> skip ;

HEADER_LOCAL  : '"' ~["\r\n]*? '.h' '"' ;
HEADER_SYSTEM : '<' ~[>\r\n]*? '.h' '>' ;

LINE_COMMENT  : '//' ~[\r\n]* -> channel(HIDDEN) ;
BLOCK_COMMENT : '/*' .*? '*/' -> channel(HIDDEN) ;
