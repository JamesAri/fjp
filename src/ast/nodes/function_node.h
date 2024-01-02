#ifndef __FUNCTION_NODE_H_
#define __FUNCTION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "types.h"

typedef std::vector<CDeclaration_Node*> parameter_list_t;
typedef std::vector<CExpression_Node*> argument_list_t;

class CFunction_Node : public CStatement_Node
{
	private:
		CType_Node *return_type;
		CIdentifier_Node *identifier;
		parameter_list_t *parameters;

		CBlock_Node *body;
	
	public:

		CFunction_Node(CType_Node *return_type, CIdentifier_Node *identifier, parameter_list_t *parameters)
			: return_type(return_type), identifier(identifier), parameters(parameters), body(nullptr)
		{
			//
		};

		void Set_Body(CBlock_Node *body)
		{
			this->body = body;
		};

		EData_Type Get_Type()
		{
			return return_type->Get_Type();
		};

		void Compile() override
		{	
			std::cout << "CFunction_Node::Compile()" << std::endl;
			
			if (!body) return;
			
			body->Compile();
		};
};

class CFunction_Call_Node : public CExpression_Node
{
	private:
		CIdentifier_Node *identifier;
		argument_list_t *arguments;

	public:

		CFunction_Call_Node(CIdentifier_Node *identifier, argument_list_t *arguments)
		{
			this->identifier = identifier;
			this->arguments = arguments;
		};

		void Compile() override
		{
			std::cout << "CFunction_Call_Node::Compile()" << std::endl;
		};
};

class CReturn_Node : public CStatement_Node
{
	private:
		CExpression_Node *expression;

	public:

		CReturn_Node()
		{
			expression = new CExpression_Node(VOID_TYPE, true);
		};

		CReturn_Node(CExpression_Node *expression) : expression(expression)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CReturn_Node::Compile()" << std::endl;
		};
};


#endif // __FUNCTION_NODE_H_