#ifndef __LOCATION_H__
#define __LOCATION_H__

// use forward declaration instead of importing from the build directory
struct YYLTYPE;

static void update_loc(YYLTYPE *loc, char *text)
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

#endif // __LOCATION_H__