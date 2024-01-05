#ifndef __IF_NODE_H_
#define __IF_NODE_H_

#include <iostream>

#include "statement_node.h"

#include "pl0.h"
#include "generators.h"

/* Compile() method logic for CIf_Node:
						
ADDR_BEFORE					(here is ADDR_BEFORE address)
if (boolean)				LIT(boolean)
{							JMC(ADDR_ELSE_BODY)
	IF BODY;				IF BODY instructions
}							JMP(ADDR_AFTER)
else [ADDR_ELSE_BODY]		ELSE BODY instructions
{							(and next instruction is on ADDR_AFTER)
	ELSE BODY;
}
ADDR_AFTER
*/


class CIf_Node : public CStatement_Node
{
	private:

		CExpression_Node *mCondition_Node;
		CStatement_Node *mIf_Statement_Node;
		CStatement_Node *mElse_Statement_Node;

	public:

		CIf_Node(CExpression_Node *condition, CStatement_Node *if_statement, CStatement_Node *else_statement = nullptr)
			: mCondition_Node(condition), mIf_Statement_Node(if_statement), mElse_Statement_Node(else_statement)
		{
			//
		};

		void Compile() override
		{	
			std::cout << "CIf_Node::Compile()" << std::endl;
			
			// LIT(boolean) value onto stack
			mCondition_Node->Compile();

			// check if conditions is not of a void type
			if (mCondition_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot use 'void' type in if condition" << std::endl;
				exit(EXIT_FAILURE);
			}

			// save JMC instruction address
			unsigned int jmc_instruction_address = sCode_Length; 

			// JMC(ADDR_ELSE_BODY) now 0, ADDR_ELSE_BODY will be set later
			// (if no else statement, ADDR_ELSE_BODY is ADDR_AFTER)
			emit_JMC(0);			

			// IF BODY instructions
			branch_compile(mIf_Statement_Node);

			if (mElse_Statement_Node) 
			{	
				unsigned int jmp_instruction_address = sCode_Length;

				// if else branch provided, we need to jump over it
				// i.e. genereate JMP instruction at the end of IF BODY
				// JMP(ADDR_AFTER) now 0, ADDR_AFTER will be set later
				emit_JMP(0);

				// sCode_Length is ADDR_ELSE_BODY
				unsigned int else_body_address = sCode_Length;

				branch_compile(mElse_Statement_Node);

				modify_param_2(jmc_instruction_address, else_body_address);

				// sCode_Length is ADDR_AFTER
				modify_param_2(jmp_instruction_address, sCode_Length);
			}
			else
			{
				// sCode_Length is ADDR_AFTER
				modify_param_2(jmc_instruction_address, sCode_Length);
			}
		};
};

#endif // __IF_NODE_H_