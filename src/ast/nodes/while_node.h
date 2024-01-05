#ifndef __WHILE_NODE_H_
#define __WHILE_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"

#include "pl0.h"
#include "identifiers.h"
#include "generators.h"

class CWhile_Node : public CStatement_Node
{
	private:

		CExpression_Node *mCondition_Node;
		CStatement_Node *mStatement_Node;

	public:

		CWhile_Node(CExpression_Node *condition, CStatement_Node *statement)
			: mCondition_Node(condition), mStatement_Node(statement)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CWhile_Node::Compile()" << std::endl;

			if (mCondition_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot use 'void' type in while loop condition" << std::endl;
				exit(EXIT_FAILURE);
			}

			unsigned int condition_address = sCode_Length;
			mCondition_Node->Compile();

			// check if condition is not of a void type
			if (mCondition_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot use 'void' type in while loop condition" << std::endl;
				exit(EXIT_FAILURE);
			}

			unsigned int jmc_instruction_address = sCode_Length; 
			emit_JMC(0);			

			branch_compile(mStatement_Node);

			emit_JMP(condition_address);

			modify_param_2(jmc_instruction_address, sCode_Length);
		};
};

#endif	// __WHILE_NODE_H_