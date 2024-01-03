#ifndef __FUNCTION_NODE_H_
#define __FUNCTION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "types.h"

#include "pl0.h"


typedef std::vector<CDeclaration_Node*> parameter_list_t;
typedef std::vector<CExpression_Node*> argument_list_t;

class CFunction_Node : public CStatement_Node
{
	private:
		CType_Node *mReturn_Type;
		CIdentifier_Node *mIdentifier;
		parameter_list_t *mParameters;

		CBlock_Node *mBody;
	
	public:

		CFunction_Node(CType_Node *return_type, CIdentifier_Node *identifier, parameter_list_t *parameters)
			: mReturn_Type(return_type), mIdentifier(identifier), mParameters(parameters), mBody(nullptr)
		{
			//
		};

		void Set_Body(CBlock_Node *body)
		{
			this->mBody = body;
		};

		EData_Type Get_Type()
		{
			return mReturn_Type->Get_Type();
		};

		void Compile() override
		{	
			std::cout << "CFunction_Node::Compile()" << std::endl;
			
			if (mBody)
			{
				mBody->Compile();
			}
		};
};

class CFunction_Call_Node : public CExpression_Node
{
	private:
		CIdentifier_Node *mIdentifier;
		argument_list_t *mArguments;

	public:

		CFunction_Call_Node(CIdentifier_Node *identifier, argument_list_t *arguments)
		{
			this->mIdentifier = identifier;
			this->mArguments = arguments;
		};

		void Compile() override
		{
			std::cout << "CFunction_Call_Node::Compile()" << std::endl;
		};
};

class CReturn_Node : public CStatement_Node
{
	private:
		CExpression_Node *mExpression;

	public:

		CReturn_Node() : mExpression(nullptr)
		{
			//
		};

		CReturn_Node(CExpression_Node *expression) : mExpression(expression)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CReturn_Node::Compile()" << std::endl;

			if (mExpression)
			{
				mExpression->Compile();
			}
		};
};


#endif // __FUNCTION_NODE_H_