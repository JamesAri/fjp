#ifndef __EXPRESSION_NODE_H_
#define __EXPRESSION_NODE_H_

#include <iostream>

#include "statement_node.h"

#include "types.h"

#include "pl0.h"
#include "generators.h"

class CExpression_Node : public CStatement_Node
{
	protected:

		EData_Type mData_Type;
		bool mIs_Constant;

	public:

		CExpression_Node()
			: mData_Type(EData_Type::UNKOWN_TYPE), mIs_Constant(true)
		{
			//
		};

		CExpression_Node(const EData_Type &type, const bool is_constant) 
			: mData_Type(type), mIs_Constant(is_constant)
		{
			//
		};

		virtual ~CExpression_Node() = default;

		virtual void Compile() override = 0;
};

// used when expression value is not retrieved and needs to be removed from the stack
// example: 
// 1+1; 				(expression value not retrieved, 2 hanging in the stack) 
// VS 
// int var = 1 + 1; 	(expression value retrieved from declaration)
class CExpression_Only_Node : public CStatement_Node
{
	protected:

		CExpression_Node *mExpression_Node;

	public:

		CExpression_Only_Node(CExpression_Node *expression)
			: mExpression_Node(expression)
		{
			//
		};

		virtual void Compile() override
		{	
			std::cout << "CExpression_Only_Node::Compile()" << std::endl;
			
			mExpression_Node->Compile();
			
			// decreases stack pointer by 1 - removes expression value from the stack
			emit_INT(-1);
		};
};

#endif // __EXPRESSION_NODE_H_