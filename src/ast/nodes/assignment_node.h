#ifndef __ASSIGNMENT_NODE_H_
#define __ASSIGNMENT_NODE_H_

#include <stdio.h>

#include "common.h"

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
	public:

		CAssignment_Node() {};

		CAssignment_Node(CExpression_Node *identifier, CExpression_Node *expression)
		{
			this->mIdentifier_Node = dynamic_cast<CIdentifier_Node*>(identifier);
			this->mExpression_Node = expression;

			// check if identifier is instance of CIdentifier_Node
			if (this->mIdentifier_Node == nullptr)
			{
				std::cout << "ERROR: you can assign only to a variable" << std::endl;
				exit(EXIT_FAILURE);
			}		
		};

		void Compile() override
		{
			std::cout << "CAssignment_Node::Compile()" << std::endl;

			// check if identifier is declared
			int index = find_identifier(mIdentifier_Node->mIdentifier.c_str());

			if (index == FAILURE)
			{
				std::cout << "ERROR: identifier not declared: " << mIdentifier_Node->mIdentifier << std::endl;
				exit(EXIT_FAILURE);
			}

			TIdentifier identifier = sIdentifier_Table[index];

			// check if identifier is not a function
			if (identifier.type == EIdentifier_Type::PROCEDURE)
			{
				std::cout << "ERROR: you cannot assign to a function" << std::endl;
				exit(EXIT_FAILURE);
			}

			// check if identifier is not constant
			if (identifier.is_constant)
			{
				std::cout << "ERROR: you cannot assign to a constant variable" << std::endl;
				exit(EXIT_FAILURE);
			}

			// compile the expression
			mExpression_Node->Compile();

			// store the value of the expression in the identifier
			emit_STO(sCurrent_Level - identifier.level, identifier.address);

			// since this is still an expression, we need to push the value of the identifier onto the stack
			// this allows to do for example following: ident1 = ident2 = expression
			// all we need to do is compile the expression again and it will be put on top of the stack
			mExpression_Node->Compile();
		};
};

#endif // __ASSIGNMENT_NODE_H_