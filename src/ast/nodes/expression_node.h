#ifndef __EXPRESSION_NODE_H_
#define __EXPRESSION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "types.h"

#include "pl0.h"

class CExpression_Node : public CStatement_Node
{
	protected:
		EData_Type type;
		bool is_constant;

	public:

		CExpression_Node() 
			: type(EData_Type::UNKOWN_TYPE), is_constant(true)
		{
			//
		};

		CExpression_Node(const EData_Type &type, const bool is_constant) 
			: type(type), is_constant(is_constant)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CExpression_Node::Compile()" << std::endl;
		};
};

#endif // __EXPRESSION_NODE_H_