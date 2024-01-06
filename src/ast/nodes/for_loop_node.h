#ifndef __FOR_LOOP_NODE_H__
#define __FOR_LOOP_NODE_H__

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"

#include "pl0.h"
#include "generators.h"


class CFor_Loop_Node : public CStatement_Node
{
	private:

		CStatement_Node *mInit_Statement_Node;
		CExpression_Node *mCondition_Node;
		// executed after each body iteration
		CExpression_Only_Node *mUpdate_Expression_Node;

		CStatement_Node *mBody_Node;

		void Validate_Compile()
		{
			if (mCondition_Node != nullptr)
			{
				// check if condition is not of a void type
				if (mCondition_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
				{
					std::cerr << "ERROR: cannot use 'void' type in for loop condition" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}

	public:

		CFor_Loop_Node(CStatement_Node *init_statement, CExpression_Node *condition, CExpression_Only_Node *update_expression)
			: mInit_Statement_Node(init_statement), mCondition_Node(condition), mUpdate_Expression_Node(update_expression), mBody_Node(nullptr)
		{
			//
		};

		void Set_Body(CStatement_Node *body)
		{
			mBody_Node = body;
		};

		void Update_Break_Statements(unsigned int address) override
		{
			mBody_Node->Update_Break_Statements(address);
		};

		void Update_Continue_Statements(unsigned int address) override
		{
			mBody_Node->Update_Continue_Statements(address);
		};

		void Compile() override
		{
			std::cout << "CFor_Loop_Node::Compile()" << std::endl;

			// (•_•)
			// ( •_•)>⌐■-■
			// (⌐■_■)
			branch_compile([&]() {
				if (mInit_Statement_Node != nullptr)
				{
					mInit_Statement_Node->Compile();
				}

				unsigned int condition_address = sCode_Length;

				if(mCondition_Node != nullptr)
				{
					mCondition_Node->Compile();
				}
				else
				{
					// if condition is not specified, then we will always produce non-zero (true) value
					emit_LIT(1);
				}

				unsigned int jmc_instruction_address = sCode_Length; 
				emit_JMC(0);

				if (mBody_Node != nullptr)
				{
					mBody_Node->Compile();
				}

				if (mUpdate_Expression_Node != nullptr)
				{
					mUpdate_Expression_Node->Compile();
				}

				emit_JMP(condition_address);

				unsigned int after_loop_address = sCode_Length;

				modify_param_2(jmc_instruction_address, after_loop_address);

				// update break and continue statements
				if (mBody_Node != nullptr)
				{
					mBody_Node->Update_Break_Statements(after_loop_address);
					mBody_Node->Update_Continue_Statements(condition_address);
				}
			});
		};
};


#endif // __FOR_LOOP_NODE_H__