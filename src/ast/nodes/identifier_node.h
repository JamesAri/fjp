#ifndef __IDENTIFIER_NODE_H_
#define __IDENTIFIER_NODE_H_

#include <iostream>

#include "tokens.h"
#include "types.h"

#include "pl0.h"
#include "generators.h"


class CIdentifier_Node : public CExpression_Node
{
	public:

		const std::string mIdentifier;

		CIdentifier_Node(const TToken_Identifier &val) 
			: mIdentifier(val.identifier)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CIdentifier_Node::Compile()" << std::endl;
		};
};

#endif // __IDENTIFIER_NODE_H_