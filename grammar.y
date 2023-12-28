%{
    extern int yylex();

    void yyerror(const char* s);
%}

/////////////
// DATA TYPES
/////////////
%token TYPE_INT
%token TYPE_CHAR
%token TYPE_BOOL
%token TYPE_VOID

/////////
// BRANCH
/////////
%token CONST
%token IF
%token ELSE
%token SWITCH
%token CASE
%token DEFAULT
%token FOR
%token DO
%token WHILE
%token BREAK
%token CONTINUE
%token RETURN

////////////
// OPERATORS
////////////
%token LOGICAL_AND
%token LOGICAL_OR
%token EQUAL
%token NOT_EQUAL
%token GREATER_EQUAL
%token LESS_EQUAL

/////////
// VALUES
/////////
%token INTEGER
%token CHAR
%token BOOL
%token IDENTIFIER

%nonassoc   IF_UNMATCHED
%nonassoc   ELSE

%right U_PLUS U_MINUS

%%
//////////
// PROGRAM
//////////
program: /* e */
    | statement_list
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
	| BREAK ';'
    | CONTINUE ';'
    ;

branch_body:
    statement
    | statement_block
    ;

/////////////// 
// IF STATEMENT
/////////////// 
if_statement:
    unmatched_if_statement
    | matched_if_statement
    ;

unmatched_if_statement:     
    IF '(' expression ')' branch_body %prec IF_UNMATCHED
    ;

matched_if_statement:
    IF '(' expression ')' branch_body ELSE branch_body
    ;

/////////////// 
// SWITCH RULES
/////////////// 
switch_statement:
    SWITCH '(' expression ')' branch_body
    ;

case_statement:
    CASE expression ':' statement
    | DEFAULT ':' statement
    ;

//////////////
// WHILE RULES
//////////////
while_statement:
    WHILE '(' expression ')' branch_body
    ;

do_while_statement:
    DO branch_body WHILE '(' expression ')'
    ;


////////////
// FOR RULES
////////////
for_statement:
    for_header branch_body
    ;

for_header:
    FOR '(' for_init_statement ';' for_expression ';' for_expression ')'
    ;

for_init_statement: /* e */
    | single_declaration
    | expression
    ;

for_expression: /* e */
    | expression
    ;


//////////////
// EXPRESSIONS
//////////////
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

/////////////// 
// DECLARATIONS   
///////////////  
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

/////////////////
// FUNCTION RULES
/////////////////
function:
    function_header statement_block
    ;

function_header:
    type identifier '(' param_list ')'
    ;

param_list: /* e */
    | single_declaration
    | param_list_ext ',' single_declaration
    ;

param_list_ext:
    single_declaration
    | param_list_ext ',' single_declaration
    ;

function_call:
    identifier '(' arg_list ')'
    ;

arg_list: /* e */
    | expression
    | arg_list_ext ',' expression
    ;

arg_list_ext:
    expression
    | arg_list_ext ',' expression
    ;

return_statement:
    RETURN expression
    | RETURN
    ;


%%
