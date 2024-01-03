#ifndef __IF_NODE_H_
#define __IF_NODE_H_

#include <iostream>

#include "statement_node.h"

#include "pl0.h"


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
			mIf_Statement_Node->Compile();
			if (mElse_Statement_Node) mElse_Statement_Node->Compile();
		};
};

#endif // __IF_NODE_H_