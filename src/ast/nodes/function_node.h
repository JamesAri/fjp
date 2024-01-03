#ifndef __FUNCTION_NODE_H_
#define __FUNCTION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"
#include "types.h"

#include "pl0.h"


typedef std::vector<CDeclaration_Node*> parameter_list_t;
typedef std::vector<CExpression_Node*> argument_list_t;

class CFunction_Node : public CStatement_Node
{
	private:
	
		CType_Node *mReturn_Type_Node;
		CIdentifier_Node *mIdentifier_Node;
		parameter_list_t *mParameter_List;

		CBlock_Node *mBody_Block_Node;
	
	public:

		CFunction_Node(CType_Node *return_type, CIdentifier_Node *identifier, parameter_list_t *parameters)
			: mReturn_Type_Node(return_type), mIdentifier_Node(identifier), mParameter_List(parameters), mBody_Block_Node(nullptr)
		{
			//
		};

		void Set_Body(CBlock_Node *body)
		{
			this->mBody_Block_Node = body;
		};

		void Compile() override
		{	
			std::cout << "CFunction_Node::Compile()" << std::endl;
			
			if (mBody_Block_Node)
			{
				mBody_Block_Node->Compile();
			}
		};
};

class CFunction_Call_Node : public CExpression_Node
{
	private:

		CIdentifier_Node *mIdentifier_Node;
		argument_list_t *mArgument_List;

	public:

		CFunction_Call_Node(CIdentifier_Node *identifier, argument_list_t *arguments)
		{
			this->mIdentifier_Node = identifier;
			this->mArgument_List = arguments;
		};

		void Compile() override
		{
			std::cout << "CFunction_Call_Node::Compile()" << std::endl;
		};
};

class CReturn_Node : public CStatement_Node
{
	private:

		CExpression_Node *mExpression_Node;

	public:

		CReturn_Node() : mExpression_Node(nullptr)
		{
			//
		};

		CReturn_Node(CExpression_Node *expression) : mExpression_Node(expression)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CReturn_Node::Compile()" << std::endl;

			if (mExpression_Node)
			{
				mExpression_Node->Compile();
			}
		};
};


#endif // __FUNCTION_NODE_H_