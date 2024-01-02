#ifndef __IF_NODE_H_
#define __IF_NODE_H_

#include <iostream>

#include "statement_node.h"

class CIf_Node : public CStatement_Node
{
	private:
		CExpression_Node *condition;
		CStatement_Node *if_statement;
		CStatement_Node *else_statement;

	public:

		CIf_Node(CExpression_Node *condition, CStatement_Node *if_statement, CStatement_Node *else_statement = nullptr)
			: condition(condition), if_statement(if_statement), else_statement(else_statement)
		{
			//
		};

		void Compile() override
		{	
			std::cout << "CIf_Node::Compile()" << std::endl;
			if_statement->Compile();
			if (else_statement) else_statement->Compile();
		};
};

#endif // __IF_NODE_H_