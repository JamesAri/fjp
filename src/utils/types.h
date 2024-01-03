#ifndef __TYPES_NODE_H_
#define __TYPES_NODE_H_

constexpr unsigned int Max_Identifier_Length = 31;

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
	VARIABLE,
	PROCEDURE
};

struct TIdentifier
{
	// +1 for terminating character
	char name[Max_Identifier_Length + 1];
	EIdentifier_Type type;
	EData_Type data_type;
	unsigned int length;
	// in case of procedure, the address is the index of the instruction in the code table
	// in case of variable, the address in the block level
	unsigned int address;
	unsigned int level;
	// used for variables only
	bool is_constant;
};

#endif // __TYPES_NODE_H_