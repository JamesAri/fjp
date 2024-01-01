%{
#include <stdio.h>
#include <stdlib.h>

// definitions with all the possible nodes
#include "ast_nodes.h"
// definitions with token structures
#include "tokens.h"

// includes needed for pure parser and location support
#include "y.tab.hpp"
#include "lex.yy.h"


extern int yylex(YYSTYPE *, YYLTYPE *);

// flex has noyywrap option, but just for compatibility reasons, leave this prototype here
int yywrap(void);

// parser prototypes (with location support)
void yyerror (YYLTYPE *, char const *);

// root node of the program
INode *rootNode = NULL;
%}

%locations

// deprecated directive
/* %pure-parser */ 
%define api.pure full

%union {
	CBlockNode* 				block_node;
	CStatementNode* 			statement_node;
	CValueNode* 				value_node;

	TTokenValue 				token_value;
	TTokenIdentifier 			token_identifier;
	
	INode* 						node;
}

%type <block_node>			program statement_block
%type <statement_node>		statement branch_body for_init_statement
%type <value_node> 			value

/* %destructor {
    if ($$ != NULL) 
	{
        delete $$;
        $$ = NULL;
    }
} */
/* <block_node> <statement_node> */

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
%token <token_value> INT
%token <token_value> FLOAT
%token <token_value> CHAR
%token <token_value> BOOL
%token <token_identifier> IDENTIFIER

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
	/* e */									{$$ = NULL; rootNode = new CBlockNode();}
    | statement_list						{$$ = NULL; rootNode = new CBlockNode();}
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
    INT													{$$ = new CValueNode($1);}
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

void yyerror (YYLTYPE *locp, char const *msg)
{
	fprintf (stderr, "%d.%d-%d.%d: %s\n",
			 locp->first_line, locp->first_column,
			 locp->last_line, locp->last_column,
			 msg);
}

int yywrap(void)
{
  return 1;
}

