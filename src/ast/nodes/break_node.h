#ifndef __BREAK_NODE_H__
#define __BREAK_NODE_H__

#include <iostream>

#include "statement_node.h"

#include "pl0.h"
#include "generators.h"


class CBreak_Node : public CStatement_Node
{	
	private:

		unsigned int mJmp_Address;
		unsigned int mJmp_Instruction_Address;

	public:

		CBreak_Node(): mJmp_Address(Invalid_Address), mJmp_Instruction_Address(Invalid_Address)
		{
			//
		};

		void Update_Break_Statements(unsigned int address) override
		{
			mJmp_Address = address;
			
			if (mJmp_Instruction_Address == Invalid_Address)
			{
				std::cerr << "COMPILER ERROR: updating break statement before compiling the node" << std::endl;
			}

			modify_param_2(mJmp_Instruction_Address, mJmp_Address);
		};

		void Compile() override
		{
			std::cout << "CBreak_Node::Compile()" << std::endl;

			mJmp_Instruction_Address = sCode_Length;
			emit_JMP(Invalid_Address);
		};
};

#endif // __BREAK_NODE_H_