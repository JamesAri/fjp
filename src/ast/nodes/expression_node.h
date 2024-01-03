#ifndef __EXPRESSION_NODE_H_
#define __EXPRESSION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "types.h"

#include "pl0.h"

class CExpression_Node : public CStatement_Node
{
	protected:
		EData_Type mType;
		bool mIs_Constant;

	public:

		CExpression_Node()
			: mType(EData_Type::UNKOWN_TYPE), mIs_Constant(true)
		{
			//
		};

		CExpression_Node(const EData_Type &type, const bool is_constant) 
			: mType(type), mIs_Constant(is_constant)
		{
			//
		};

		virtual ~CExpression_Node() = default;

		virtual void Compile() override = 0;
};

#endif // __EXPRESSION_NODE_H_