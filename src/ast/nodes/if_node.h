#ifndef __IF_NODE_H_
#define __IF_NODE_H_

#include <iostream>

#include "statement_node.h"

#include "pl0.h"


class CIf_Node : public CStatement_Node
{
	private:
		CExpression_Node *mCondition;
		CStatement_Node *mIf_Statement;
		CStatement_Node *mElse_Statement;

	public:

		CIf_Node(CExpression_Node *condition, CStatement_Node *if_statement, CStatement_Node *else_statement = nullptr)
			: mCondition(condition), mIf_Statement(if_statement), mElse_Statement(else_statement)
		{
			//
		};

		void Compile() override
		{	
			std::cout << "CIf_Node::Compile()" << std::endl;
			mIf_Statement->Compile();
			if (mElse_Statement) mElse_Statement->Compile();
		};
};

#endif // __IF_NODE_H_