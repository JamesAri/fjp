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
	EIdentifier_Type type;
	EData_Type data_type;
	// +1 for terminating character
	char name[Max_Identifier_Length + 1];
	// In case of procedure, the address is the index of the instruction in the code table.
	// In case of variable, the address is the level in the stack frame (block level).
	unsigned int address;
	// level - level of current block scope (0 is global)
	unsigned int level;
	// branch level - used when declaring variables in the same level 
	// but due to branching (if-else, while, ...), they can have the 
	// same name as variables in the same level but not in the same branch
	unsigned int branch_level;
	// used for variables only
	bool is_constant;
	// When we leave scope, we will mark the identifier as deleted because
	// we want to keep the identifier in the table, so we can for example log it.
	// Should be removed in the future.
	bool is_deleted; 
};

#endif // __TYPES_NODE_H_