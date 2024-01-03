#ifndef __IDENTIFIER_NODE_H_
#define __IDENTIFIER_NODE_H_

#include <iostream>

#include "tokens.h"
#include "types.h"

#include "pl0.h"


class CIdentifier_Node : public CExpression_Node
{
	private:
		std::string mIdentifier;
		
	public:

		CIdentifier_Node(const TToken_Identifier &val) 
			: mIdentifier(val.identifier)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CIdentifier_Node::Compile()" << std::endl;
		};

		std::string Get_Name()
		{
			return mIdentifier;
		};
};

#endif // __IDENTIFIER_NODE_H_