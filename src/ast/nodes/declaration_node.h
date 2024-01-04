#ifndef __DECLARATION_NODE_H_
#define __DECLARATION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"
#include "identifier_node.h"
#include "type_node.h"

#include "pl0.h"
#include "generators.h"

class CDeclaration_Node;

namespace
{
	typedef std::vector<CDeclaration_Node*> declaration_list_t;
}


class CDeclaration_Node : public CStatement_Node
{
	private:
		
		CType_Node *mType_Node;
		CIdentifier_Node *mIdentifier_Node;
		bool mIs_Constant;
		
		// nullptr when declaration is without definition
		CExpression_Node *mExpression_Node;

	public:

		CDeclaration_Node(CType_Node *type, CIdentifier_Node *identifier, const bool is_constant, CExpression_Node *expression = nullptr)
			: mIs_Constant(is_constant), mExpression_Node(expression)
		{
			this->mType_Node = type;
			this->mIdentifier_Node = identifier;
		};

		void Set_Expression(CExpression_Node *expression)
		{
			this->mExpression_Node = expression;
		};

		EData_Type Get_Type()
		{
			return mType_Node->mData_Type;
		};

		bool Is_Constant()
		{
			return mIs_Constant;
		};

		void Compile() override
		{
			std::cout << "CDeclaration_Node::Compile()" << std::endl;

			const int address = sCurrent_Block_Address;
			const int level = sCurrent_Level;

			add_identifier(
				mIdentifier_Node->mIdentifier.c_str(), 
				EIdentifier_Type::VARIABLE, 
				mType_Node->mData_Type, 
				address, 
				level,
				sCurrent_Branch_Level,
				mIs_Constant
			);

			sCurrent_Block_Address++;

			// increment stack pointer by 1 => variable will have random value that was
			// previsouly on the stack, but if expression was provided, we will overwrite it
			// in the next step
			emit_INT(1); // NOTE: this might change with the introduction of arrays

			if (mExpression_Node)
			{
				mExpression_Node->Compile();
				// level must be relative (note for Mira: precti si prednasku o generovani kodu)
				emit_STO(sCurrent_Level - level, address);
			}
		};
};

class CMulti_Declaration_Node : public CStatement_Node
{
	private:
		
		EData_Type mData_Type;
		declaration_list_t mDeclarations;
		bool mAre_Constants;

	public:

		// we retreive the type and constness information from the first declaration
		CMulti_Declaration_Node(CDeclaration_Node *declaration)
			: mData_Type(declaration->Get_Type()), mAre_Constants(declaration->Is_Constant())
		{
			mDeclarations.push_back(declaration);
		};

		void Add_Declaration(CIdentifier_Node *identifier, CExpression_Node *expression = nullptr)
		{	
			CDeclaration_Node *decl = new CDeclaration_Node(new CType_Node(mData_Type), identifier, mAre_Constants, expression);
			mDeclarations.push_back(decl);
		};

		void Compile() override
		{
			std::cout << "CMulti_Declaration_Node::Compile()" << std::endl;

			for (auto declaration : mDeclarations)
			{
				declaration->Compile();
			}
		};
};

#endif // __DECLARATION_NODE_H_