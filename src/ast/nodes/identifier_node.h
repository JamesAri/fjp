#ifndef __IDENTIFIER_NODE_H_
#define __IDENTIFIER_NODE_H_

#include <iostream>

#include "tokens.h"
#include "types.h"

#include "pl0.h"


class CIdentifier_Node : public CExpression_Node
{
	private:
		std::string identifier;
		
	public:

		CIdentifier_Node(const TToken_Identifier &val) 
			: identifier(val.identifier)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CIdentifier_Node::Compile()" << std::endl;
		};

		std::string Get_Name()
		{
			return identifier;
		};
};

#endif // __IDENTIFIER_NODE_H_