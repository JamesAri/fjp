#ifndef __DECLARATION_NODE_H_
#define __DECLARATION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"
#include "identifier_node.h"
#include "type_node.h"

#include "pl0.h"
#include "generators.h"


class CDeclaration_Node : public CStatement_Node
{
	private:
		
		CType_Node *mType_Node;
		CIdentifier_Node *mIdentifier_Node;
		bool mIs_Constant;
		
		// nullptr when declaration is without definition
		CExpression_Node *mExpression_Node;

		void Validate_Compile()
		{
			if (mExpression_Node != nullptr)
			{
				// check if expression is not void
				if (mExpression_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
				{
					std::cerr << "ERROR: cannot assign 'void' type to a variable" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		}

	public:

		CDeclaration_Node(CType_Node *type, CIdentifier_Node *identifier, const bool is_constant, CExpression_Node *expression = nullptr)
			: mType_Node(type), mIdentifier_Node(identifier), mIs_Constant(is_constant), mExpression_Node(expression)
		{
			if(type->mData_Type == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot declare a variable of 'void' type: " << identifier->mIdentifier << std::endl;
				exit(EXIT_FAILURE);
			}
		};

		void Set_Expression(CExpression_Node *expression)
		{
			this->mExpression_Node = expression;
		};

		EData_Type Get_Type()
		{
			return mType_Node->mData_Type;
		};

		std::string Get_Identifier_Name()
		{
			return mIdentifier_Node->mIdentifier;
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

			// increment stack pointer by 1 (allocate space for the variable)
			emit_INT(1); // NOTE: this might change with the introduction of arrays

			if (mExpression_Node != nullptr)
			{
				// push expression value onto stack
				mExpression_Node->Compile();
			}
			else
			{
				// set the default value to zero
				emit_LIT(0);
			}
			
			// store the default or expression value in the variable
			// level must be relative (note for Mira: precti si prednasku o generovani kodu)
			emit_STO(sCurrent_Level - level, address);

			Validate_Compile();
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