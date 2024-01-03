#ifndef __DECLARATION_NODE_H_
#define __DECLARATION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "identifier_node.h"
#include "type_node.h"

#include "pl0.h"

class CDeclaration_Node;

typedef std::vector<CDeclaration_Node*> declaration_list_t;

class CDeclaration_Node : public CStatement_Node
{
	private:
		CType_Node *type;
		bool is_constant;
		
		CIdentifier_Node *identifier;
		// nullptr when declaration is without definition
		CExpression_Node *expression;
	public:

		CDeclaration_Node(CType_Node *type, CIdentifier_Node *identifier, const bool is_constant, CExpression_Node *expression = nullptr)
			: is_constant(is_constant), expression(expression)
		{
			this->type = type;
			this->identifier = identifier;
		};

		void Set_Expression(CExpression_Node *expression)
		{
			this->expression = expression;
		};

		EData_Type Get_Type()
		{
			return type->Get_Type();
		};

		bool Is_Constant()
		{
			return is_constant;
		};

		void Compile() override
		{
			std::cout << "CDeclaration_Node::Compile()" << std::endl;
			std::cout << "identifier: " << identifier->Get_Name() << std::endl;

			add_identifier(identifier->Get_Name().c_str(), EIdentifier_Type::VARIABLE, type->Get_Type(), sCurrent_Block_Address, sCurrent_Level, is_constant);
			sCurrent_Block_Address++;
		};
};

class CMulti_Declaration_Node : public CStatement_Node
{
	private:
		EData_Type type;
		declaration_list_t *declarations;
		bool are_constant;

	public:

		CMulti_Declaration_Node(CDeclaration_Node *declaration)
			: type(declaration->Get_Type()), are_constant(declaration->Is_Constant()) 
		{
			//
		};

		void Add_Declaration(CIdentifier_Node *identifier, CExpression_Node *expression = nullptr)
		{	
			CDeclaration_Node *decl = new CDeclaration_Node(new CType_Node(type), identifier, are_constant, expression);
			declarations->push_back(decl);
		};

		void Compile() override
		{
			std::cout << "CMulti_Declaration_Node::Compile()" << std::endl;

		};
};

#endif // __DECLARATION_NODE_H_