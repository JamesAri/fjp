#ifndef __SWITCH_NODE_H__
#define __SWITCH_NODE_H__

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"

#include "node_lists.h"

#include "pl0.h"
#include "generators.h"

class CCase_Statement_Node : public CStatement_Node
{
	private:
		
		CExpression_Node *mExpression_Node;
		statement_list_t *mStatement_List;
		bool mIs_Default;

	public:
		
		CCase_Statement_Node(CExpression_Node *expression, statement_list_t *statements, bool is_default)
			: mExpression_Node(expression), mStatement_List(statements), mIs_Default(is_default)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CCase_Statement_Node::Compile()" << std::endl;

		};
		
};

class CSwitch_Node : public CStatement_Node
{
	private:

		CExpression_Node *mExpression_Node;
		statement_list_t *mSwitch_Body_Node;

	public:

		CSwitch_Node(CExpression_Node *condition,  statement_list_t *switch_body)
			: mExpression_Node(condition), mSwitch_Body_Node(switch_body)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CSwitch_Node::Compile()" << std::endl;

			// nothing to do here
		};
};

#endif // __SWITCH_NODE_H__