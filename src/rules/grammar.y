%{
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <vector>

// definitions with all the possible nodes
#include "ast_nodes.h"
// definitions with token structures
#include "tokens.h"
// definitions with type structures
#include "types.h"
// definitions with operators
#include "operators.h"
// definitions with identifiers
#include "identifiers.h"
// pl0 definitions
#include "pl0.h"

// includes needed for pure parser and location support
#include "y.tab.hpp"
#include "lex.yy.h"

// reade more about yylex: https://www.ibm.com/docs/en/zos/2.1.0?topic=works-yyparse-yylex
// forward declaration of yylex for compatibility reasons
extern int yylex(YYSTYPE *, YYLTYPE *); 

// flex has noyywrap option, but just to be sure, leave this prototype here (some bison versions need it)
int yywrap(void);

// parser prototypes (with location support)
void yyerror (YYLTYPE *, char const *);

// root node of the program
CBlock_Node *sRootNode = nullptr;

// Create code storage and generator helpers
code_t sCode;
// Will be correctly initialized with Compile_Init from root node
int sCurrent_Level;
int sCurrent_Branch_Level;
unsigned int sCode_Length;
unsigned int sCurrent_Block_Address;

// Create identifier table and initialize count
identifier_table_t sIdentifier_Table;
unsigned int sIdentifier_Count = 0;
%}

%locations

// deprecated directive
/* %pure-parser */ 
%define api.pure full

%union {
	// Abstract nodes
	CStatement_Node* 			statement_node;
	CExpression_Node* 			expression_node;
	CExpression_Only_Node* 		expression_only_node;
	CBranch_Body_Node* 			branch_body_node;

	// AST nodes
	CBlock_Node* 				block_node;

	CDeclaration_Node* 			declaration_node;
	CMulti_Declaration_Node* 	multi_declaration_node;

	CType_Node* 				type_node;
	CIdentifier_Node* 			identifier_node;
	CValue_Node* 				value_node;

	CFunction_Node* 			function_node;
	CFunction_Call_Node* 		function_call_node;
	CReturn_Node* 				return_node;

	CIf_Node* 					if_node;

	CWhile_Node* 				while_node;
	CDo_While_Node* 			do_while_node;

	CFor_Loop_Node* 			for_loop_node;

	// Token containers
	TToken_Value 				token_value;
	TToken_Identifier 			token_identifier;

	// Lists
	statement_list_t* 			statement_list;
	declaration_list_t* 		parameter_list;
	expression_list_t* 			argument_list;
}

%type <statement_node>			statement for_init_statement
%type <expression_node>			expression expression_value expression_operation for_expression
%type <branch_body_node> 		branch_body

%type <block_node>				program statement_block

%type <declaration_node>		single_declaration
%type <multi_declaration_node>	multi_declaration

%type <type_node>				type
%type <identifier_node>			identifier
%type <value_node> 				value

%type <function_node>			function function_header
%type <function_call_node>		function_call
%type <return_node>				return_statement

%type <if_node>					if_statement unmatched_if_statement matched_if_statement

%type <while_node>				while_statement
%type <do_while_node>			do_while_statement

%type <for_loop_node>			for_statement for_header
%type <expression_only_node>	for_expression_only

%type <statement_list>			statement_list
%type <parameter_list>			parameter_list function_parameters
%type <argument_list>			argument_list function_argmunets

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
%token TYPE_FLOAT
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
	/* e */									{$$ = NULL; sRootNode = new CBlock_Node();}
    | statement_list						{$$ = NULL; sRootNode = new CBlock_Node($1);}
    ;

statement_list:
    statement								{$$ = new statement_list_t(); $$->push_back($1);}
    | statement_list statement				{$$ = $1; $$->push_back($2);}
    | statement_block						{$$ = new statement_list_t(); $$->push_back($1);}
    | statement_list statement_block		{$$ = $1; $$->push_back($2);}
    ;

statement_block:
    '{' '}'									{$$ = new CBlock_Node();}
    | '{' statement_list '}'				{$$ = new CBlock_Node($2);}
    ;

statement:
    ';'										{$$ = new CEmpty_Statement_Node();}
    | expression ';'						{$$ = new CExpression_Only_Node($1);}
    | single_declaration ';'				{$$ = $1;}
    | multi_declaration ';'					{$$ = $1;}
    | if_statement							{$$ = $1;}
    | switch_statement						{}
    | case_statement						{}
    | while_statement						{$$ = $1;}
    | do_while_statement ';'				{$$ = $1;}
    | for_statement							{$$ = $1;}
    | function								{$$ = $1;}
    | return_statement ';'					{$$ = $1;}
	| BREAK ';'								{$$ = new CBreak_Node();}
    | CONTINUE ';'							{$$ = new CContinue_Node();}
    ;

branch_body:
    statement								{$$ = new CBranch_Body_Node($1);}
    | statement_block						{$$ = new CBranch_Body_Node($1);}
    ;

/////////////// 
// IF STATEMENT
/////////////// 
if_statement:
    unmatched_if_statement					{$$ = $1;}
    | matched_if_statement					{$$ = $1;}
    ;

unmatched_if_statement:     
    IF '(' expression ')' branch_body %prec IF_UNMATCHED	{$$ = new CIf_Node($3, $5);}
    ;

matched_if_statement:
    IF '(' expression ')' branch_body ELSE branch_body		{$$ = new CIf_Node($3, $5, $7);}
    ;

//////////////
// EXPRESSIONS
//////////////
expression:
    expression_value									{$$ = $1;}
    | expression_operation								{$$ = $1;}
    ;

expression_value:
    '(' expression ')'									{$$ = $2;}
    | identifier										{$$ = $1;}
    | value												{$$ = $1;}	
    | function_call										{$$ = $1;}
    ;

expression_operation:
    expression '=' expression							{$$ = new CAssignment_Node($1, $3);}
    | expression '+' expression							{$$ = new CArithmetic_Operation_Node($1, $3, EArithmetic_Operation::ADD);}
    | expression '-' expression							{$$ = new CArithmetic_Operation_Node($1, $3, EArithmetic_Operation::SUB);}
    | expression '*' expression							{$$ = new CArithmetic_Operation_Node($1, $3, EArithmetic_Operation::MUL);}
    | expression '/' expression							{$$ = new CArithmetic_Operation_Node($1, $3, EArithmetic_Operation::DIV);}
    | expression '%' expression							{$$ = new CArithmetic_Operation_Node($1, $3, EArithmetic_Operation::MOD);}
    | expression LOGICAL_AND expression					{$$ = new CLogical_Operation_Node($1, $3, ELogical_Operation::AND);}
    | expression LOGICAL_OR expression					{$$ = new CLogical_Operation_Node($1, $3, ELogical_Operation::OR);}
    | expression EQUAL expression						{$$ = new CRelational_Operation_Node($1, $3, ERelational_Operation::EQUAL);}
    | expression NOT_EQUAL expression					{$$ = new CRelational_Operation_Node($1, $3, ERelational_Operation::NOT_EQUAL);}
    | expression '<' expression							{$$ = new CRelational_Operation_Node($1, $3, ERelational_Operation::LESS_THAN);}
    | expression LESS_EQUAL expression					{$$ = new CRelational_Operation_Node($1, $3, ERelational_Operation::LESS_THAN_EQUAL);}
    | expression '>' expression							{$$ = new CRelational_Operation_Node($1, $3, ERelational_Operation::GREATER_THAN);}
    | expression GREATER_EQUAL expression				{$$ = new CRelational_Operation_Node($1, $3, ERelational_Operation::GREATER_THAN_EQUAL);}
    | '+' expression %prec U_PLUS						{$$ = new CUnary_Operation_Node($2, EUnary_Operation::PLUS);}
    | '-' expression %prec U_MINUS						{$$ = new CUnary_Operation_Node($2, EUnary_Operation::MINUS);}
    | '!' expression									{$$ = new CUnary_Operation_Node($2, EUnary_Operation::NOT);}
    ;

/////////////// 
// DECLARATIONS   
///////////////  
single_declaration:
    type identifier										{$$ = new CDeclaration_Node($1, $2, false);}
    | CONST type identifier								{$$ = new CDeclaration_Node($2, $3, true);}
    | type identifier '=' expression					{$$ = new CDeclaration_Node($1, $2, false, $4);}
    | CONST type identifier '=' expression				{$$ = new CDeclaration_Node($2, $3, true, $5);}
    ;

multi_declaration:
    single_declaration ',' identifier					{$$ = new CMulti_Declaration_Node($1); $$->Add_Declaration($3);}
    | single_declaration ',' identifier '=' expression	{$$ = new CMulti_Declaration_Node($1); $$->Add_Declaration($3, $5);}
    | multi_declaration ',' identifier					{$$ = $1; $$->Add_Declaration($3);}
    | multi_declaration ',' identifier '=' expression	{$$ = $1; $$->Add_Declaration($3, $5);}
    ;

type:
    TYPE_INT											{$$ = new CType_Node(EData_Type::INT_TYPE);}
	| TYPE_FLOAT										{$$ = new CType_Node(EData_Type::FLOAT_TYPE);}
    | TYPE_CHAR 										{$$ = new CType_Node(EData_Type::CHAR_TYPE);}
    | TYPE_BOOL 										{$$ = new CType_Node(EData_Type::BOOL_TYPE);}
    | TYPE_VOID 										{$$ = new CType_Node(EData_Type::VOID_TYPE);}
    ;

value:
    INT													{$$ = new CValue_Node($1, EData_Type::INT_TYPE); delete $1.value;}
	| FLOAT												{$$ = new CValue_Node($1, EData_Type::FLOAT_TYPE); delete $1.value;}
    | CHAR 												{$$ = new CValue_Node($1, EData_Type::CHAR_TYPE); delete $1.value;}
    | BOOL 												{$$ = new CValue_Node($1, EData_Type::BOOL_TYPE); delete $1.value;}
    ;

identifier:
    IDENTIFIER											{$$ = new CIdentifier_Node($1); delete $1.identifier;}
    ;


/////////////// 
// SWITCH RULES
/////////////// 
switch_statement:
    SWITCH '(' expression ')' branch_body				{}
    ;

case_statement:
    CASE expression ':' statement						{}
    | DEFAULT ':' statement								{}
    ;

//////////////
// WHILE RULES
//////////////
while_statement:
    WHILE '(' expression ')' branch_body				{$$ = new CWhile_Node($3, $5);}
    ;

do_while_statement:
    DO branch_body WHILE '(' expression ')'				{$$ = new CDo_While_Node($5, $2);}
    ;


////////////
// FOR RULES
////////////
for_statement:
    for_header branch_body								{$$ = $1; $1->Set_Body($2);}
    ;

for_header:
    FOR '(' for_init_statement ';' for_expression ';' for_expression_only ')'	{$$ = new CFor_Loop_Node($3, $5, $7);}
    ;

for_init_statement: 
	/* e */												{$$ = nullptr;}
    | single_declaration								{$$ = $1;}
    | expression										{$$ = new CExpression_Only_Node($1);}
    ;

for_expression: 
	/* e */												{$$ = nullptr;}
    | expression										{$$ = $1;}
    ;

for_expression_only:
	/* e */												{$$ = nullptr;}
    | expression										{$$ = new CExpression_Only_Node($1);}
    ;

/////////////////
// FUNCTION RULES
/////////////////
function:
    function_header statement_block						{$$ = $1; $$->Set_Body($2);}
    ;

function_header:
    type identifier '(' function_parameters ')'			{$$ = new CFunction_Node($1, $2, $4);}
    ;

function_parameters:
	/* e */												{$$ = new declaration_list_t();}
	| parameter_list									{$$ = $1;}
	;	

parameter_list: 										
    single_declaration									{$$ = new declaration_list_t(); $$->push_back($1);}
    | parameter_list ',' single_declaration				{$$ = $1; $$->push_back($3);}
    ;

function_call:
    identifier '(' function_argmunets ')'				{$$ = new CFunction_Call_Node($1, $3);}
    ;

function_argmunets:
	/* e */												{$$ = new expression_list_t();}
	| argument_list										{$$ = $1;}
	;	

argument_list: 
    expression											{$$ = new expression_list_t(); $$->push_back($1);}
    | argument_list ',' expression						{$$ = $1; $$->push_back($3);}
    ;

return_statement:
    RETURN expression									{$$ = new CReturn_Node($2);}
    | RETURN											{$$ = new CReturn_Node();}
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

