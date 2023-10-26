%{
    extern int yylex();

    void yyerror(const char* s);
%}

%union {
    Location location;
    Token token;
}

// Data types
%token <location> TYPE_INT
%token <location> TYPE_CHAR
%token <location> TYPE_BOOL
%token <location> TYPE_VOID

// Branch
%token <location> CONST
%token <location> IF
%token <location> ELSE
%token <location> SWITCH
%token <location> CASE
%token <location> DEFAULT
%token <location> FOR
%token <location> DO
%token <location> WHILE
%token <location> BREAK
%token <location> CONTINUE
%token <location> RETURN

// Operators
%token <location> LOGICAL_AND
%token <location> LOGICAL_OR
%token <location> EQUAL
%token <location> NOT_EQUAL
%token <location> GREATER_EQUAL
%token <location> LESS_EQUAL

// Values
%token <token> INTEGER
%token <token> CHAR
%token <token> BOOL
%token <token> IDENTIFIER

%nonassoc   IF_UNMAT
%nonassoc   ELSE

%%
program:
    statement_list
    ;

statement_list:
    statement
    | statement_list statement
    | statement_block
    | statement_list statement_block
    ;

statement_block:
    '{' '}'
    | '{' statement_list '}'
    ;

statement:
    ';'
    | BREAK ';'
    | CONTINUE ';'
    | expression ';'
    | single_declaration ';'
    | multi_declaration ';'
    | if_statement
    | switch_statement
    | case_statement
    | while_statement
    | do_while_statement ';'
    | for_statement
    | function
    | return_statement ';'
    ;

branch_body:
    statement
    | statement_block
    ;

// If statement
if_statement:
    unmatched_if_statement
    | matched_if_statement
    ;

unmatched_if_statement:     
    IF '(' expression ')' branch_body %prec IF_UNMAT
    ;

matched_if_statement:
    IF '(' expression ')' branch_body ELSE branch_body
    ;


// Expressions
expression:
    expression_value
    | expression_operation
    ;

expression_value:
    '(' expression ')'
    | value
    | function_call
    ;

expression_operation:
    expression '=' expression
    | expression '+' expression
    | expression '-' expression
    | expression '*' expression
    | expression '/' expression
    | expression '%' expression
    | expression LOGICAL_AND expression
    | expression LOGICAL_OR expression
    | expression '>' expression
    | expression GREATER_EQUAL expression
    | expression '<' expression
    | expression LESS_EQUAL expression
    | expression EQUAL expression
    | expression NOT_EQUAL expression
    | '+' expression %prec U_PLUS
    | '-' expression %prec U_MINUS
    | '!' expression
    ;

// DECLARATIONS    
single_declaration:
    type identifier
    | CONST type identifier
    | type identifier '=' expression
    | CONST type identifier '=' expression
    ;

multi_declaration:
    single_declaration ',' identifier
    | single_declaration ',' identifier '=' expression
    | multi_declaration ',' identifier
    | multi_declaration ',' identifier '=' expression
    ;

type:
    TYPE_INT    
    | TYPE_CHAR 
    | TYPE_BOOL 
    | TYPE_VOID 
    ;

value:
    INTEGER 
    | CHAR 
    | BOOL 
    ;

identifier:
    IDENTIFIER 
    ;

%%
