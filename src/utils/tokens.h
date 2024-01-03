#ifndef __TOKENS_H_
#define __TOKENS_H_

// Token types used when parsing the source code (in lexer)

struct TToken_Value
{
	char *value;
};

struct TToken_Identifier
{
	char *identifier;
};

#endif // __TOKENS_H_