#ifndef __MODIFIERS_H_
#define __MODIFIERS_H_

#include "pl0.h"

#include <iostream>

#include "nodes/statement_node.h"

// UTILITY FUNCTIONS

// searches for identifier in specified scope and branch level and their
// nested scopes/branches and returns the most recent identifier index
inline int find_identifier(const char *name, const unsigned int level = 0, const unsigned int branch_level = 0)
{
	// we want the most recent identifier, thus iterate backwards
	for (int i = sIdentifier_Count - 1; i >= 0; i--)
	{	
		if (sIdentifier_Table[i].is_deleted) continue;

		// if identifier is in scope level above (i.e. parent scope), return FAILURE
		// this is because we want to ignore identifiers in parent scopes
		if (sIdentifier_Table[i].level < level)
		{
			return FAILURE;
		}
		
		// Same logic as above, but for nested branches (for example nested if statements).
		// Additionally, we want to check only identifiers in the same scope level 
		// as level logic is already handled above.
		if(sIdentifier_Table[i].level == level && sIdentifier_Table[i].branch_level < branch_level)
		{
			return FAILURE;
		}

		if (strcmp(sIdentifier_Table[i].name, name) == 0)
		{
			return i;
		}
	}
	
	return FAILURE;
}

// MODIFIERS

// for given scope level, marks all identifiers in the scope as deleted
inline void mark_scope_identifiers_as_deleted(int level)
{
	for (int i = sIdentifier_Count - 1; i >= 0; i--)
	{	
		// Skip deleted identifiers - identifiers with larger scope level (nested scopes) already cleaned up at this point.
		if (sIdentifier_Table[i].is_deleted) continue;

		if (sIdentifier_Table[i].level == level)
		{
			std::cout << "Deleting identifier (scope): " << sIdentifier_Table[i].name << std::endl;
			sIdentifier_Table[i].is_deleted = true;
		}
		else break;
	}
}

// for given scope and branch level, marks all identifiers in the branch as deleted
inline void mark_branch_identifiers_as_deleted(int level, int branch_level)
{
	for (int i = sIdentifier_Count - 1; i >= 0; i--)
	{	
		if (sIdentifier_Table[i].is_deleted) continue;

		if (sIdentifier_Table[i].level == level && sIdentifier_Table[i].branch_level == branch_level)
		{
			std::cout << "Deleting identifier (branch): " << sIdentifier_Table[i].name << std::endl;
			sIdentifier_Table[i].is_deleted = true;
		}
		else break;
	}
}

inline void add_identifier(
	const char *name, 
	const EIdentifier_Type type, 
	EData_Type data_type, 
	const unsigned int address, 
	const unsigned int level, 
	const unsigned int branch_level, 
	const bool is_constant,
	const unsigned int number_of_parameters = 0
	)
{	
	if (sIdentifier_Count >= Max_Identifier_Table_Length)
	{
		std::cout << "ERROR: Identifier table overflow" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (find_identifier(name, level, branch_level) != FAILURE)
	{
		std::cout << "ERROR: Identifier \"" << name <<  "\" already exists" << std::endl;
		exit(EXIT_FAILURE);
	}

	TIdentifier *identifier = &sIdentifier_Table[sIdentifier_Count];
	
	identifier->type = type;
	identifier->data_type = data_type;
	identifier->address = address;
	identifier->level = level;
	identifier->branch_level = branch_level;
	identifier->is_constant = is_constant;
	identifier->number_of_parameters = number_of_parameters;
	
	for (unsigned int i = 0; i < Max_Identifier_Length; i++)
	{
		identifier->name[i] = name[i];
		
		if (name[i] == '\0')
		{
			break;
		}
	}
	identifier->name[Max_Identifier_Length] = '\0';
	
	sIdentifier_Count++;
}

inline void modify_param_1(const unsigned int address, const int param_1)
{
	sCode[address].param_1 = param_1;
}

inline void modify_param_2(const unsigned int address, const int param_2)
{
	TCode_Entry_Value value;
	value.i = param_2;
	sCode[address].param_2 = value;
}

inline void emit(const instruction_t instruction, const int param_1, const TCode_Entry_Value param_2, const bool is_float)
{
	sCode[sCode_Length].instruction = instruction;
	sCode[sCode_Length].param_1 = param_1;
	sCode[sCode_Length].param_2 = param_2;
	sCode[sCode_Length].is_float = is_float;
	sCode_Length++;
}

inline void emit(const instruction_t instruction, const int param_1, const int param_2)
{
	TCode_Entry_Value value;
	value.i = param_2;
	emit(instruction, param_1, value, false);
}

inline void emit(const instruction_t instruction, const int param_1, const float param_2)
{
	TCode_Entry_Value value;
	value.f = param_2;
	emit(instruction, param_1, value, true);
}

inline void emit_LIT(const int value)
{
	emit(PL0::LIT, 0, value);
}

inline void emit_LIT(const float value)
{
	emit(PL0::LIT, 0, value);
}

inline void emit_OPR(const operation_t operation)
{
	emit(PL0::OPR, 0, static_cast<int>(operation));
}

inline void emit_LOD(const int level, const int address)
{
	emit(PL0::LOD, level, address);
}

inline void emit_STO(const int level, const int address)
{
	emit(PL0::STO, level, address);
}

inline void emit_CAL(const int level, const int address)
{
	emit(PL0::CAL, level, address);
}

inline void emit_INT(const int value)
{
	emit(PL0::INT, 0, value);
}

inline void emit_JMP(const int address)
{
	emit(PL0::JMP, 0, address);
}

inline void emit_JMC(const int address)
{
	emit(PL0::JMC, 0, address);
}

inline void emit_RET()
{
	emit(PL0::RET, 0, 0);
}

inline void emit_REA()
{
	emit(PL0::REA, 0, 0);
}

inline void emit_WRI()
{
	emit(PL0::WRI, 0, 0);
}

inline void emit_NEW()
{
	emit(PL0::NEW, 0, 0);
}

inline void emit_DEL()
{
	emit(PL0::DEL, 0, 0);
}

inline void emit_LDA()
{
	emit(PL0::LDA, 0, 0);
}

inline void emit_STA()
{
	emit(PL0::STA, 0, 0);
}

inline void emit_PLD()
{
	emit(PL0::PLD, 0, 0);
}

inline void emit_PST()
{
	emit(PL0::PST, 0, 0);
}


// SCOPE UTILS

// prepares new scope for given node and compiles it
inline void scope_compile(CStatement_Node *statement)
{
	sCurrent_Level++;

	unsigned int saved_current_block_address = sCurrent_Block_Address;
	
	sCurrent_Block_Address = 3;

	// for PL/0 control structures
	emit_INT(sCurrent_Block_Address);

	statement->Compile();

	sCurrent_Block_Address = saved_current_block_address;

	// mark all identifiers in current scope as deleted
	mark_scope_identifiers_as_deleted(sCurrent_Level);
		
	sCurrent_Level--;
}

// prepares new branch enviroment for given node and compiles it
inline void branch_compile(CStatement_Node *statement)
{
	sCurrent_Branch_Level++;

	statement->Compile();


	// mark all identifiers in current branch as deleted
	mark_branch_identifiers_as_deleted(sCurrent_Level, sCurrent_Branch_Level);
	
	sCurrent_Branch_Level--;
}

#endif // __MODIFIERS_H_