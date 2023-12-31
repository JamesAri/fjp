%{
#include <stdio.h>
#include <stdlib.h>

#include "nodes.h"

void yyerror(const char* s);
int yywrap(void);
extern int yylex();
%}

%union {
	CBlockNode* block_node;
	CStatementNode* statement_node;
	
	INode* node;
}

%type <block_node>			program statement_block
%type <statement_node>		statement branch_body for_init_statement

/////////////
// DATA TYPES
/////////////
%token <node> TYPE_INT
%token <node> TYPE_CHAR
%token <node> TYPE_BOOL
%token <node> TYPE_VOID

/////////
// BRANCH
/////////
%token <node> CONST
%token <node> IF
%token <node> ELSE
%token <node> SWITCH
%token <node> CASE
%token <node> DEFAULT
%token <node> FOR
%token <node> DO
%token <node> WHILE
%token <node> BREAK
%token <node> CONTINUE
%token <node> RETURN

////////////
// OPERATORS
////////////
%token <node> LOGICAL_AND
%token <node> LOGICAL_OR
%token <node> EQUAL
%token <node> NOT_EQUAL
%token <node> GREATER_EQUAL
%token <node> LESS_EQUAL

/////////
// VALUES
/////////
%token <node> INT
%token <node> FLOAT
%token <node> CHAR
%token <node> BOOL
%token <node> IDENTIFIER

//////////////
// ASSOC/PREC
//////////////
%right      '='
%left       LOGICAL_OR
%left       LOGICAL_AND
%left       EQUAL NOT_EQUAL
%left       LESS_EQUAL GREATER_EQUAL '<' '>'
%left       '-' '+'
%left       '*' '/' '%'
%right      '!'
%right      U_PLUS U_MINUS

%nonassoc   IF_UNMATCHED
%nonassoc   ELSE

%%
//////////
// ROOT
//////////
program: 
	/* e */									{}
    | statement_list						{}
    ;

statement_list:
    statement								{}
    | statement_list statement				{}
    | statement_block						{}
    | statement_list statement_block		{}
    ;

statement_block:
    '{' '}'									{}
    | '{' statement_list '}'				{}
    ;

statement:
    ';'										{}
    | expression ';'						{}
    | single_declaration ';'				{}
    | multi_declaration ';'					{}
    | if_statement							{}
    | switch_statement						{}
    | case_statement						{}
    | while_statement						{}
    | do_while_statement ';'				{}
    | for_statement							{}
    | function								{}
    | return_statement ';'					{}
	| BREAK ';'								{}
    | CONTINUE ';'							{}
    ;

branch_body:
    statement								{}
    | statement_block						{}
    ;

/////////////// 
// IF STATEMENT
/////////////// 
if_statement:
    unmatched_if_statement					{}
    | matched_if_statement					{}
    ;

unmatched_if_statement:     
    IF '(' expression ')' branch_body %prec IF_UNMATCHED	{}
    ;

matched_if_statement:
    IF '(' expression ')' branch_body ELSE branch_body		{}
    ;

/////////////// 
// SWITCH RULES
/////////////// 
switch_statement:
    SWITCH '(' expression ')' branch_body					{}
    ;

case_statement:
    CASE expression ':' statement							{}
    | DEFAULT ':' statement									{}
    ;

//////////////
// WHILE RULES
//////////////
while_statement:
    WHILE '(' expression ')' branch_body					{}
    ;

do_while_statement:
    DO branch_body WHILE '(' expression ')'					{}
    ;


////////////
// FOR RULES
////////////
for_statement:
    for_header branch_body									{}
    ;

for_header:
    FOR '(' for_init_statement ';' for_expression ';' for_expression ')'	{}
    ;

for_init_statement: 
	/* e */												{}
    | single_declaration								{}
    | expression										{}
    ;

for_expression: 
	/* e */												{}
    | expression										{}
    ;


//////////////
// EXPRESSIONS
//////////////
expression:
    expression_value									{}
    | expression_operation								{}
    ;

expression_value:
    '(' expression ')'									{}
    | identifier												{}		
    | value												{}		
    | function_call										{}
    ;

expression_operation:
    expression '=' expression							{}
    | expression '+' expression							{}
    | expression '-' expression							{}
    | expression '*' expression							{}
    | expression '/' expression							{}
    | expression '%' expression							{}
    | expression LOGICAL_AND expression					{}
    | expression LOGICAL_OR expression					{}
    | expression '>' expression							{}
    | expression GREATER_EQUAL expression				{}
    | expression '<' expression							{}
    | expression LESS_EQUAL expression					{}
    | expression EQUAL expression						{}
    | expression NOT_EQUAL expression					{}
    | '+' expression %prec U_PLUS						{}
    | '-' expression %prec U_MINUS						{}
    | '!' expression									{}
    ;

/////////////// 
// DECLARATIONS   
///////////////  
single_declaration:
    type identifier										{}
    | CONST type identifier								{}
    | type identifier '=' expression					{}
    | CONST type identifier '=' expression				{}
    ;

multi_declaration:
    single_declaration ',' identifier					{}
    | single_declaration ',' identifier '=' expression	{}
    | multi_declaration ',' identifier					{}
    | multi_declaration ',' identifier '=' expression	{}
    ;

type:
    TYPE_INT											{}
    | TYPE_CHAR 										{}
    | TYPE_BOOL 										{}
    | TYPE_VOID 										{}
    ;

value:
    INT													{}
	| FLOAT												{}
    | CHAR 												{}
    | BOOL 												{}
    ;

identifier:
    IDENTIFIER											{}
    ;

/////////////////
// FUNCTION RULES
/////////////////
function:
    function_header statement_block						{}
    ;

function_header:
    type identifier '(' param_list ')'					{}
    ;

param_list: 
	/* e */												{}
    | single_declaration								{}
    | param_list_ext ',' single_declaration				{}
    ;

param_list_ext:
    single_declaration									{}
    | param_list_ext ',' single_declaration				{}
    ;

function_call:
    identifier '(' arg_list ')'							{}
    ;

arg_list: 
	/* e */												{}
    | expression										{}
    | arg_list_ext ',' expression						{}
    ;

arg_list_ext:
    expression											{}
    | arg_list_ext ',' expression						{}
    ;

return_statement:
    RETURN expression									{}
    | RETURN											{}
    ;


%%

void yyerror (char const *s) {
	fprintf (stderr, "%s\n", s);
}

int yywrap(void)
{
  return 1;
}

