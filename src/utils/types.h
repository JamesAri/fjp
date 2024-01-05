#ifndef __TYPES_NODE_H_
#define __TYPES_NODE_H_

// inclusive
constexpr unsigned int Max_Identifier_Length = 31;

enum class EData_Type
{
	INT_TYPE,
	FLOAT_TYPE,
	CHAR_TYPE,
	BOOL_TYPE,
	VOID_TYPE,
	UNKNOWN_TYPE
};

enum class EIdentifier_Type
{
	VARIABLE,
	PROCEDURE
};

struct TFunction_Parameter
{
	EData_Type data_type;
	bool is_constant;
};

typedef std::vector<TFunction_Parameter> function_parameter_list_t;

struct TIdentifier
{
	EIdentifier_Type type;
	EData_Type data_type;
	// +1 for terminating character
	char name[Max_Identifier_Length + 1];
	// In case of procedure, the address is the index of the instruction in the code table.
	// In case of variable, the address is the level in the stack frame (block level).
	unsigned int address;
	// level of current block scope (0 is global)
	unsigned int level;
	// branch level used when declaring variables in the same level 
	// when branching (if-else, while, ...), identifiers can have the 
	// same name as identifiers in the same level but not in the same branch
	unsigned int branch_level;
	// used for variables only
	bool is_constant;
	// When we leave scope, we will mark the identifier as deleted because
	// we want to keep the identifier in the table, so we can for example log it.
	// Should be removed in the future.
	bool is_deleted; 
	// used for procedures only
	unsigned int number_of_parameters;
	// function_parameter_list_t parameters;
};

#endif // __TYPES_NODE_H_