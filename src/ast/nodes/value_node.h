#ifndef __VALUE_NODE_H_
#define __VALUE_NODE_H_

#include <iostream>

#include "expression_node.h"
#include "tokens.h"


class CValue_Node : public CExpression_Node
{	
	private:
		TToken_Value tokenValue;
		
	public:

		CValue_Node(const TToken_Value &val, const EData_Type &type) 
			: CExpression_Node(type, true), tokenValue(val)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CValue_Node::Compile()" << std::endl;
		};
};

#endif // __VALUE_NODE_H_