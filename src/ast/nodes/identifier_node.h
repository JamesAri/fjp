#ifndef __IDENTIFIER_NODE_H_
#define __IDENTIFIER_NODE_H_

#include <iostream>

#include "common.h"

#include "expression_node.h"

#include "tokens.h"
#include "types.h"

#include "pl0.h"
#include "generators.h"
#include "identifiers.h"


class CIdentifier_Node : public CExpression_Node
{
	public:

		const std::string mIdentifier;

		CIdentifier_Node(const TToken_Identifier &val) 
			: mIdentifier(val.identifier)
		{
			//
		};

		TIdentifier Find_Declared_Identifier(std::string error_message)
		{
			int index = find_identifier(mIdentifier.c_str());

			if (index == FAILURE)
			{
				std::cerr << error_message << std::endl;
				exit(EXIT_FAILURE);
			}

			TIdentifier identifier = sIdentifier_Table[index];

			// ! UPDATE NODE DATA ! - this is important for the parent node to know what type of data is on the stack
			mData_Type = identifier.data_type;
			mIs_Constant = identifier.is_constant;

			return identifier;
		};

		void Compile() override
		{
			std::cout << "CIdentifier_Node::Compile()" << std::endl;

			TIdentifier identifier = Find_Declared_Identifier(
				"ERROR: undeclared identifier: " + mIdentifier
			);

			if (identifier.type == EIdentifier_Type::PROCEDURE)
			{
				std::cerr << "ERROR: you cannot use a function name as an expression: " << identifier.name << std::endl;
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
					std::cerr << "ERROR: cannot use void type as an expression: " << identifier.name << std::endl;
					exit(EXIT_FAILURE);
				default:
					std::cerr << "ERROR: unknown identifier type " << identifier.name << std::endl;
					exit(EXIT_FAILURE);
			}
		};
};

#endif // __IDENTIFIER_NODE_H_