#ifndef __SWITCH_NODE_H__
#define __SWITCH_NODE_H__

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"
#include "block_node.h"

#include "node_lists.h"

#include "pl0.h"
#include "generators.h"

// TODO: Refactor this horrific mess

class CCase_Statement_Node : public CStatement_Node
{
	private:
		
		CExpression_Node *mExpression_Node;
		CStatement_Node *mStatement_Node;
		bool mIs_Default;

	public:

		// used for default case
		CCase_Statement_Node(CStatement_Node *statement)
			: mExpression_Node(nullptr), mStatement_Node(statement), mIs_Default(true)
		{
			if(statement == nullptr)
			{
				std::cerr << "COMPILER ERROR: statement in case statement is null" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		// used for case statements
		CCase_Statement_Node(CExpression_Node *expression, CStatement_Node *statement)
			: mExpression_Node(expression), mStatement_Node(statement), mIs_Default(false)
		{
			if (expression == nullptr)
			{
				std::cerr << "COMPILER ERROR: expression in case statement is null" << std::endl;
				exit(EXIT_FAILURE);
			}

			if (statement == nullptr)
			{
				std::cerr << "COMPILER ERROR: statement in case statement is null" << std::endl;
				exit(EXIT_FAILURE);
			}
		};

		bool Is_Default()
		{
			return mIs_Default;
		}

		void Compile_Statement()
		{
			mStatement_Node->Compile();
		};

		void Compile_Expression()
		{
			mExpression_Node->Compile();

			if (mExpression_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot use 'void' type in case statement expression" << std::endl;
				exit(EXIT_FAILURE);
			}
		};

		void Compile() override
		{
			std::cout << "CCase_Statement_Node::Compile()" << std::endl;

			std::cerr << "ERROR: cannot compile case statement outside of switch statement" << std::endl;
		};
		
};

class CSwitch_Node : public CStatement_Node
{
	private:

		CExpression_Node *mExpression_Node;
		CStatement_Node *mSwitch_Body_Node;

		void Validate_Compile()
		{
			if (mExpression_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot use 'void' type in switch header expression" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

		void Compile_Single_Statement()
		{
			CCase_Statement_Node *case_statement_node = dynamic_cast<CCase_Statement_Node*>(mSwitch_Body_Node);

			if (case_statement_node == nullptr)
			{
				std::cout << "ERROR: switch body is missing case statement" << std::endl;
				exit(EXIT_FAILURE);
			}
			
			if(case_statement_node->Is_Default())
			{
				case_statement_node->Compile_Statement();
			}
			else
			{
				mExpression_Node->Compile();

				case_statement_node->Compile_Expression();

				emit_OPR(PL0::Operations::EQ);

				unsigned int jmc_instruction_address = sCode_Length;
				emit_JMC(0);

				case_statement_node->Compile_Statement();

				modify_param_2(jmc_instruction_address, sCode_Length);
			}
			Validate_Compile();
		}

	public:

		CSwitch_Node(CExpression_Node *expression,  CStatement_Node *switch_body)
			: mExpression_Node(expression), mSwitch_Body_Node(switch_body)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CSwitch_Node::Compile()" << std::endl;

			CBlock_Node *block_node = dynamic_cast<CBlock_Node*>(mSwitch_Body_Node);

			if (block_node == nullptr)
			{
				// if switch body is not a block node, it should be just once case/default statement
				Compile_Single_Statement();
				return;
			}

			for (statement_list_t::iterator it = block_node->mStatement_List->begin(); it != block_node->mStatement_List->end(); ++it)
			{
				CCase_Statement_Node *case_statement_node = dynamic_cast<CCase_Statement_Node*>(*it);

				if (case_statement_node == nullptr)
				{
					std::cerr << "ERROR: switch body has statement outside of case label" << std::endl;
					exit(EXIT_FAILURE);
				}

				if(case_statement_node->Is_Default())
				{
					case_statement_node->Compile_Statement();
					
					while (true)
					{
						it++;

						if (it == block_node->mStatement_List->end())
						{
							break;
						}

						CCase_Statement_Node *case_statement_node = dynamic_cast<CCase_Statement_Node*>(*it);

						if (case_statement_node != nullptr)
						{
							std::cerr << "ERROR: case statement after default statement in switch body" << std::endl;
							exit(EXIT_FAILURE);
						}

						(*it)->Compile();
					}

					break;
				}

				mExpression_Node->Compile();

				case_statement_node->Compile_Expression();

				emit_OPR(PL0::Operations::EQ);

				unsigned int jmc_instruction_address = sCode_Length;
				emit_JMC(0);

				case_statement_node->Compile_Statement();

				// compile other statements under the same case label
				while (true)
				{
					it++;

					if (it == block_node->mStatement_List->end())
					{
						break;
					}

					CCase_Statement_Node *case_statement_node = dynamic_cast<CCase_Statement_Node*>(*it);

					if (case_statement_node != nullptr)
					{
						it--;
						break;
					}

					(*it)->Compile();
				}

				modify_param_2(jmc_instruction_address, sCode_Length);
			}

			Validate_Compile();
		};
};

#endif // __SWITCH_NODE_H__