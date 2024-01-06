#ifndef __DO_WHILE_NODE_H_
#define __DO_WHILE_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"

#include "pl0.h"
#include "generators.h"

class CDo_While_Node : public CStatement_Node
{
	private:

		CExpression_Node *mCondition_Node;
		CStatement_Node *mStatement_Node;

	public:

		CDo_While_Node(CExpression_Node *condition, CStatement_Node *statement)
			: mCondition_Node(condition), mStatement_Node(statement)
		{
			//
		};

		void Update_Break_Statements(unsigned int address) override
		{
			mStatement_Node->Update_Break_Statements(address);
		};

		void Compile() override
		{
			std::cout << "CDo_While_Node::Compile()" << std::endl;

			if (mCondition_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot use 'void' type in do while loop condition" << std::endl;
				exit(EXIT_FAILURE);
			}

			unsigned int body_address = sCode_Length;

			branch_compile(mStatement_Node);

			mCondition_Node->Compile();

			// negate condition
			emit_LIT(0);
			emit_OPR(PL0::Operations::EQ);
			
			emit_JMC(body_address);
		};
};

#endif	// __DO_WHILE_NODE_H_