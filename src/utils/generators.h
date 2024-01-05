#ifndef __MODIFIERS_H_
#define __MODIFIERS_H_

#include <iostream>

#include "nodes/statement_node.h"

#include "identifiers.h"
#include "pl0.h"

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