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
			: mData_Type(EData_Type::UNKNOWN_TYPE), mIs_Constant(true)
		{
			//
		};

		CExpression_Node(const CExpression_Node &expression)
			: mData_Type(expression.mData_Type), mIs_Constant(expression.mIs_Constant)
		{
			//
		};

		CExpression_Node(const EData_Type &type, const bool is_constant) 
			: mData_Type(type), mIs_Constant(is_constant)
		{
			//
		};

		EData_Type Get_Data_Type() const
		{
			return mData_Type;
		};

		virtual ~CExpression_Node() = default;

		virtual void Compile() override = 0;
};

// used when expression value is not retrieved and needs to be removed from the stack
// example: 
// 1+1; 				(expression value not retrieved, 2 hanging in the stack) 
// OR
// a = b = 1;			(assignment is an expression, so this will result with hanging 1 on the top of the stack)
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

			if (mExpression_Node->Get_Data_Type() != EData_Type::VOID_TYPE)
			{
				// decreases stack pointer by 1 - removes non-void expression value from the stack
				emit_INT(-1);
			}
		};
};

#endif // __EXPRESSION_NODE_H_