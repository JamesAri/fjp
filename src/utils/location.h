#ifndef __LOCATION_H__
#define __LOCATION_H__

struct YYLTYPE;

static void update_loc(YYLTYPE *yylloc, char *yytext)
{
	yylloc->first_line = yylloc->last_line;
    yylloc->first_column = yylloc->last_column;
    for(int i = 0; yytext[i] != '\0'; i++) {
        if(yytext[i] == '\n') {
            yylloc->last_line++;
            yylloc->last_column = 0;
        }
        else {
            yylloc->last_column++;
        }
    }
}

#endif // __LOCATION_H__