#ifndef __IDENTIFIERS_H_
#define __IDENTIFIERS_H_

#include <iostream>

#include "common.h"

#include "types.h"

#include "pl0.h"

// IDENTIFIER STORAGE

constexpr unsigned int Max_Identifier_Table_Length = 500;

typedef TIdentifier identifier_table_t[Max_Identifier_Table_Length];

extern identifier_table_t sIdentifier_Table;
extern unsigned int sIdentifier_Count;


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
		std::cerr << "ERROR: Identifier table overflow" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (find_identifier(name, level, branch_level) != FAILURE)
	{
		std::cerr << "ERROR: Identifier \"" << name <<  "\" already exists" << std::endl;
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

#endif // __IDENTIFIERS_H_