#ifndef __CONTINUE_NODE_H__
#define __CONTINUE_NODE_H__

#include <iostream>

#include "statement_node.h"

#include "pl0.h"
#include "generators.h"

class CContinue_Node : public CStatement_Node
{	
	private:
		
		unsigned int mJmp_Address;
		unsigned int mJmp_Instruction_Address;

	public:

		static unsigned int gCounter;


		CContinue_Node(): mJmp_Address(Invalid_Address), mJmp_Instruction_Address(Invalid_Address)
		{
			//
		};

		void Update_Continue_Statements(unsigned int address) override
		{
			if(mJmp_Address == Invalid_Address)
			{
				gCounter--;
			}

			mJmp_Address = address;

			if (mJmp_Instruction_Address == Invalid_Address)
			{
				std::cerr << "COMPILER ERROR: updating 'continue' statement before compiling the node" << std::endl;
			}

			modify_param_2(mJmp_Instruction_Address, mJmp_Address);
		};

		void Compile() override
		{
			std::cout << "CContinue_Node::Compile()" << std::endl;

			gCounter++;
			
			mJmp_Instruction_Address = sCode_Length;
			emit_JMP(Invalid_Address);
		};
};

#endif // __CONTINUE_NODE_H__