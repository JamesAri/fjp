#ifndef __ASSIGNMENT_NODE_H_
#define __ASSIGNMENT_NODE_H_

#include <stdio.h>

#include "statement_node.h"
#include "expression_node.h"
#include "identifier_node.h"

#include "pl0.h"
#include "generators.h"


class CAssignment_Node : public CExpression_Node
{
	private:

		CIdentifier_Node *mIdentifier_Node;
		CExpression_Node *mExpression_Node;

		void Validate_Compile(TIdentifier identifier)
		{
			// check if identifier is not of a void type
			if (identifier.data_type == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot assign to a 'void' type" << std::endl;
				exit(EXIT_FAILURE);
			}

			// check if identifier is not a function
			if (identifier.type == EIdentifier_Type::PROCEDURE)
			{
				std::cerr << "ERROR: you cannot assign to a function" << std::endl;
				exit(EXIT_FAILURE);
			}

			// check if identifier is not constant
			if (identifier.is_constant)
			{
				std::cerr << "ERROR: you cannot assign to a constant" << std::endl;
				exit(EXIT_FAILURE);
			}

			// check if expression is not void
			if (mExpression_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot assign 'void' type to a variable" << std::endl;
				exit(EXIT_FAILURE);
			}
		}

	public:

		CAssignment_Node(CExpression_Node *identifier, CExpression_Node *expression)
		{
			this->mIdentifier_Node = dynamic_cast<CIdentifier_Node*>(identifier);
			this->mExpression_Node = expression;

			// check if identifier is instance of CIdentifier_Node
			if (this->mIdentifier_Node == nullptr)
			{
				std::cerr << "ERROR: you can assign only to a variable" << std::endl;
				exit(EXIT_FAILURE);
			}		
		};
		

		void Compile() override
		{
			std::cout << "CAssignment_Node::Compile()" << std::endl;

			TIdentifier identifier = mIdentifier_Node->Find_Declared_Identifier(
				"ERROR: cannot assign to an undeclared variable: " + mIdentifier_Node->mIdentifier
			);

			// compile the expression
			mExpression_Node->Compile();

			// if variable has non-float type, but the expression has float type, round down to the nearest integer
			if (mExpression_Node->Get_Data_Type() == EData_Type::FLOAT_TYPE && identifier.data_type != EData_Type::FLOAT_TYPE)
			{
				emit_LIT(1);
				emit_OPR(PL0::Operations::DIV);
			}

			// store the value of the expression in the identifier
			emit_STO(sCurrent_Level - identifier.level, identifier.address);
			
			// since this is still an expression, we need to push the value of the identifier onto the stack
			// this for example allows us to do following: ident1 = ident2 = expression
			// all we need to do is load the value back onto the stack
			emit_LOD(sCurrent_Level - identifier.level, identifier.address);

			// validate the assignment
			Validate_Compile(identifier);
		};
};

#endif // __ASSIGNMENT_NODE_H_