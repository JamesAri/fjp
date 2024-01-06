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
			: mData_Type(EData_Type::UNKNOWN), mIs_Constant(true)
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

		void Set_Data_Type(const EData_Type &type)
		{
			mData_Type = type;
		};

		bool Is_Constant() const
		{
			return mIs_Constant;
		};

		void Set_Constant(const bool is_constant)
		{
			mIs_Constant = is_constant;
		};

		virtual ~CExpression_Node() = default;

		virtual void Compile() override = 0;
};

// used when expression value is not retrieved and needs to be removed from the stack
// example: 
//
// 1+1; 				binary operation is an expression and is value not retrieved (value 2 hanging on the top of stack)
// OR
// a = b = 1;			assignment is an expression, so this will result with hanging value 1 on top of the stack
//
// VS 
//
// int var = 1 + 1; 	expression value retrieved from declaration
// OR
// for(; i<10;;) 		binary operation is an expression, but value is retrieved by the for-loop logic 
//						when checking the condition - it's basically if(i<10)
//						
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

		void Update_Break_Statements(unsigned int address) override
		{
			//
		};

		virtual void Compile() override
		{	
			std::cout << "CExpression_Only_Node::Compile()" << std::endl;
			
			mExpression_Node->Compile();

			// void type expression should not generate any value on the stack
			if (mExpression_Node->Get_Data_Type() != EData_Type::VOID_TYPE)
			{
				// decreases stack pointer by 1 - removes non-void expression value from the stack
				emit_INT(-1);
			}
		};
};

#endif // __EXPRESSION_NODE_H_