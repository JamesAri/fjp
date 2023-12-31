%option noyywrap
%{
#include "nodes.h"

#include "y.tab.hpp"
%}

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
<INITIAL>{INTEGER}      return INT;
<INITIAL>{FLOAT}        return FLOAT;
<INITIAL>(\'.\')        return CHAR;
<INITIAL>"true"         return BOOL;
<INITIAL>"false"        return BOOL;
<INITIAL>{IDENTIFIER}   return IDENTIFIER;

%{
/////////////// 
// OTHER TOKENS
///////////////
%}
<INITIAL>{LINE_COMMENT}		;
<INITIAL>"/*"               BEGIN BLOCK_COMMENT;
<BLOCK_COMMENT>"*/"         BEGIN INITIAL;
<BLOCK_COMMENT>.            ;

<INITIAL>.                  return yytext[0];

%%