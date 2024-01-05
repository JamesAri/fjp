#ifndef __IDENTIFIER_NODE_H_
#define __IDENTIFIER_NODE_H_

#include <iostream>

#include "common.h"

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

			int index = find_identifier(mIdentifier.c_str());

			if (index == FAILURE)
			{
				std::cout << "ERROR: undeclared identifier " << mIdentifier << std::endl;
				exit(EXIT_FAILURE);
			}

			TIdentifier identifier = sIdentifier_Table[index];

			if (identifier.type == EIdentifier_Type::PROCEDURE)
			{
				std::cout << "ERROR: you cannot use a function name as an expression: " << identifier.name << std::endl;
				exit(EXIT_FAILURE);
			}

			switch (identifier.data_type)
			{
				case EData_Type::INT_TYPE:
				case EData_Type::FLOAT_TYPE:
				case EData_Type::CHAR_TYPE:
				case EData_Type::BOOL_TYPE:
					emit_LOD(sCurrent_Level - identifier.level, identifier.address);
					break;
				case EData_Type::VOID_TYPE:
					std::cout << "ERROR: cannot use void type as an expression: " << identifier.name << std::endl;
					exit(EXIT_FAILURE);
				default:
					std::cout << "ERROR: unknown identifier type " << identifier.name << std::endl;
					exit(EXIT_FAILURE);
			}
		};
};

#endif // __IDENTIFIER_NODE_H_