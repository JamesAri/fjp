%{
// definitions with all the possible nodes
#include "ast_nodes.h"

#include "y.tab.hpp"

// forward declaration of our utility functions (defined at the bottom)
void save_value(YYSTYPE *);
void save_indetifier(YYSTYPE *);
static void update_loc(YYLTYPE *, char *);

// updates the location of the token, so we can have meaningful error messages from parser
#define YY_USER_ACTION update_loc(yylloc, yytext);

%}
%option warn noyywrap
%option bison-bridge bison-locations
/* Creates file taht contains function prototypes, extern variables, and types used by the scanner */
%option header-file="lex.yy.h"

%s BLOCK_COMMENT

DIGIT       [0-9]
INTEGER     [0-9]+
FLOAT       (([0-9]*\.[0-9]+)|([0-9]+\.[0-9]*))
LETTER      [a-zA-Z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
LINE_COMMENT "//"(.)*
%%

%{
////////////////////
// DATA TYPES TOKENS
////////////////////
%}
<INITIAL>"int"   return TYPE_INT;
<INITIAL>"char"  return TYPE_CHAR;
<INITIAL>"bool"  return TYPE_BOOL;
<INITIAL>"void"  return TYPE_VOID;

%{
////////////////
// BRANCH TOKENS
//////////////// 
%}
<INITIAL>"const"    return CONST;
<INITIAL>"if"       return IF;
<INITIAL>"else"     return ELSE;
<INITIAL>"switch"   return SWITCH;
<INITIAL>"case"     return CASE;
<INITIAL>"default"  return DEFAULT;
<INITIAL>"for"      return FOR;
<INITIAL>"do"       return DO;
<INITIAL>"while"    return WHILE;
<INITIAL>"break"    return BREAK;
<INITIAL>"continue" return CONTINUE;
<INITIAL>"return"   return RETURN;

%{
////////////////// 
// OPERATOR TOKENS
////////////////// 
%}
<INITIAL>"=="   return EQUAL;
<INITIAL>"!="   return NOT_EQUAL;
<INITIAL>">="   return GREATER_EQUAL;
<INITIAL>"<="   return LESS_EQUAL;
<INITIAL>"&&"   return LOGICAL_AND;
<INITIAL>"||"   return LOGICAL_OR;
<INITIAL>[-+*/%&!<>=(){}[\],:;] return yytext[0];

%{
//////////////// 
// VALUES TOKENS
////////////////
%}
<INITIAL>{INTEGER}      save_value(yylval); return INT;
<INITIAL>{FLOAT}        save_value(yylval); return FLOAT;
<INITIAL>(\'.\')        save_value(yylval); return CHAR;
<INITIAL>"true"         save_value(yylval); return BOOL;
<INITIAL>"false"        save_value(yylval); return BOOL;
<INITIAL>{IDENTIFIER}   save_indetifier(yylval); return IDENTIFIER;

%{
/////////////// 
// OTHER TOKENS
///////////////
%}
<INITIAL>{LINE_COMMENT}		;
<INITIAL>"/*"               BEGIN BLOCK_COMMENT;
<BLOCK_COMMENT>"*/"         BEGIN INITIAL;
<BLOCK_COMMENT>.            ;

<INITIAL>[ \t\r\n]+      	; /* skip whitespace which are not part of a string */
<INITIAL>.                  return yytext[0];

%%

void save_value(YYSTYPE *yylval) 
{
	yylval->token_value.value = strdup(yytext);
}

void save_indetifier(YYSTYPE *yylval) 
{
	yylval->token_identifier.identifier = strdup(yytext);
}

void update_loc(YYLTYPE *loc, char *text)
{
	loc->first_line = loc->last_line;
    loc->first_column = loc->last_column;
    for(int i = 0; text[i] != '\0'; i++) {
        if(text[i] == '\n') {
            loc->last_line++;
            loc->last_column = 0;
        }
        else {
            loc->last_column++;
        }
    }
}