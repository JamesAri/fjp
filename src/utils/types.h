#ifndef __TYPES_NODE_H_
#define __TYPES_NODE_H_

enum EData_Type
{
	INT_TYPE,
	FLOAT_TYPE,
	CHAR_TYPE,
	BOOL_TYPE,
	VOID_TYPE,
	UNKOWN_TYPE
};

enum EIdentifier_Type
{
	VARIABLE_IDENTIFIER,
	PROCEDURE_IDENTIFIER
};

struct TIdentifier
{
	EIdentifier_Type type;
	unsigned int address;
	unsigned int size;
	// unsigned int level;
};

#endif // __TYPES_NODE_H_