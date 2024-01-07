#ifndef __READ_NODE_H__
#define __READ_NODE_H__

#include <iostream>

#include "statement_node.h"
#include "identifiers.h"

#include "pl0.h"
#include "generators.h"

class CRead_Node : public CStatement_Node
{
	public:

		CIdentifier_Node *mIdentifier_Node;

		CRead_Node(CIdentifier_Node *identifier) : mIdentifier_Node(identifier)
		{
			this->mIdentifier_Node = dynamic_cast<CIdentifier_Node*>(identifier);

			// check if identifier is instance of CIdentifier_Node
			if (this->mIdentifier_Node == nullptr)
			{
				std::cerr << "ERROR: you need to provide non-const variable in order to read from the input" << std::endl;
				exit(EXIT_FAILURE);
			}	
		};

		void Compile() override
		{
			std::cout << "CRead_Node::Compile()" << std::endl;

			TIdentifier identifier = mIdentifier_Node->Find_Declared_Identifier(
				"ERROR: cannot read value to an undeclared variable: " + mIdentifier_Node->mIdentifier
			);
			
			emit_REA();

			emit_STO(sCurrent_Level - identifier.level, identifier.address);
		};
};


#endif	// __READ_NODE_H__