#ifndef __FOR_LOOP_NODE_H__
#define __FOR_LOOP_NODE_H__

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"
#include "declaration_node.h"

#include "pl0.h"
#include "generators.h"


class CFor_Loop_Node : public CStatement_Node
{
	private:

		CDeclaration_Node *mDeclaration_Node;
		CExpression_Node *mCondition_Node;
		// executed after each body iteration
		CExpression_Node *mExpression_Node;

		CStatement_Node *mBody_Node;

	public:

		CFor_Loop_Node(CDeclaration_Node *declaration, CExpression_Node *condition, CExpression_Node *expression)
			: mDeclaration_Node(declaration), mCondition_Node(condition), mExpression_Node(expression), mBody_Node(nullptr)
		{
			//
		};

		void Set_Body(CStatement_Node *body)
		{
			mBody_Node = body;
		};

		void Compile() override
		{
			std::cout << "CFor_Loop_Node::Compile()" << std::endl;

			// (•_•)
			// ( •_•)>⌐■-■
			// (⌐■_■)
			branch_compile([&]() {
				mDeclaration_Node->Compile();

				unsigned int condition_address = sCode_Length;
				mCondition_Node->Compile();

				// check if condition is not of a void type
				if (mCondition_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
				{
					std::cerr << "ERROR: cannot use 'void' type in for loop condition" << std::endl;
					exit(EXIT_FAILURE);
				}

				unsigned int jmc_instruction_address = sCode_Length; 
				emit_JMC(0);

				if (mBody_Node != nullptr)
				{
					mBody_Node->Compile();
				}

				mExpression_Node->Compile();

				emit_JMP(condition_address);

				modify_param_2(jmc_instruction_address, sCode_Length);
			});
		};
};


#endif // __FOR_LOOP_NODE_H__