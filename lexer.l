%s BLOCK_COMMENT

DIGIT       [0-9]
INTEGER     [0-9]+
LETTER      [a-zA-Z_]
IDENTIFIER  {LETTER}({LETTER}|{DIGIT})*
LINE_COMMENT "//"(.)*
%%

%{
// DATA TYPES TOKENS
%}
<INITIAL>"int"   something(); return TYPE_INT;
<INITIAL>"char"  something(); return TYPE_CHAR;
<INITIAL>"bool"  something(); return TYPE_BOOL;
<INITIAL>"void"  something(); return TYPE_VOID;

%{
// BRANCH TOKENS
%}
<INITIAL>"const"    something(); return CONST;
<INITIAL>"if"       something(); return IF;
<INITIAL>"else"     something(); return ELSE;
<INITIAL>"switch"   something(); return SWITCH;
<INITIAL>"case"     something(); return CASE;
<INITIAL>"default"  something(); return DEFAULT;
<INITIAL>"for"      something(); return FOR;
<INITIAL>"do"       something(); return DO;
<INITIAL>"while"    something(); return WHILE;
<INITIAL>"break"    something(); return BREAK;
<INITIAL>"continue" something(); return CONTINUE;
<INITIAL>"return"   something(); return RETURN;

%{
// OPERATOR TOKENS
%}
<INITIAL>"=="   something(); return EQUAL;
<INITIAL>"!="   something(); return NOT_EQUAL;
<INITIAL>">="   something(); return GREATER_EQUAL;
<INITIAL>"<="   something(); return LESS_EQUAL;
<INITIAL>"&&"   something(); return LOGICAL_AND;
<INITIAL>"||"   something(); return LOGICAL_OR;
<INITIAL>[-+*/%&!<>=(){}[\],:;] something(); return yytext[0];

%{
// VALUES TOKENS
%}
<INITIAL>{INTEGER}      something(); return INT;
<INITIAL>(\'.\')        something(); return CHAR;
<INITIAL>"true"         something(); return BOOL;
<INITIAL>"false"        something(); return BOOL;
<INITIAL>{IDENTIFIER}   something(); return IDENTIFIER;

%{
// OTHER TOKENS
%}

%%

int yywrap() {
    return 1;
}
