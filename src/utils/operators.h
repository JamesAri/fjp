#ifndef __OPERATORS_NODE_H_
#define __OPERATORS_NODE_H_

enum class EArithmetic_Operation
{
	ADD,
	SUB,
	MUL,
	DIV,
	MOD,
};

enum class ELogical_Operation
{
	AND,
	OR,
};

enum class ERelational_Operation
{
	EQUAL,
	NOT_EQUAL,
	LESS_THAN,
	LESS_THAN_EQUAL,
	GREATER_THAN,
	GREATER_THAN_EQUAL,
};

enum class EUnary_Operation
{
	PLUS,
	MINUS,
	NOT,
};

#endif // __OPERATORS_NODE_H_