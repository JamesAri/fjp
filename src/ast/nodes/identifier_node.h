#ifndef __IDENTIFIER_NODE_H_
#define __IDENTIFIER_NODE_H_

#include <iostream>

#include "tokens.h"
#include "types.h"

class CIdentifier_Node : public CExpression_Node
{
	private:
		TToken_Identifier identifier;
		
	public:

		CIdentifier_Node(const TToken_Identifier &val, const EData_Type &type, const bool &is_constant) 
			: CExpression_Node(type, is_constant), identifier(val)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CIdentifier_Node::Compile()" << std::endl;
		};
};

#endif // __IDENTIFIER_NODE_H_