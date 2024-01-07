#ifndef __WRITE_NODE_H__
#define __WRITE_NODE_H__

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"

#include "pl0.h"
#include "generators.h"


class CWrite_Node : public CStatement_Node
{
	private:

		CExpression_Node *mExpression_Node;

	public:

		CWrite_Node(CExpression_Node *expression) : mExpression_Node(expression)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CWrite_Node::Compile()" << std::endl;

			mExpression_Node->Compile();

			if (mExpression_Node->Get_Data_Type() != EData_Type::CHAR_TYPE)
			{
				std::cerr << "ERROR: cannot write non-char type to console" << std::endl;
				exit(EXIT_FAILURE);
			}

			emit_WRI();
		};
};

#endif	// __WRITE_NODE_H__