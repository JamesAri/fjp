#ifndef __VALUE_NODE_H_
#define __VALUE_NODE_H_

#include <iostream>

#include "expression_node.h"
#include "tokens.h"

#include "pl0.h"

class CValue_Node : public CExpression_Node
{	
	private:
		std::string mToken_Value;
		
	public:

		CValue_Node(const TToken_Value &val, const EData_Type &type) 
			: CExpression_Node(type, true), mToken_Value(val.value)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CValue_Node::Compile()" << std::endl;
			
			int val;
			switch (mType)
			{
				case EData_Type::INT_TYPE:
					emit_LIT(std::stoi(mToken_Value));
					break;
				case EData_Type::FLOAT_TYPE:
					emit_LIT(std::stof(mToken_Value));
					break;
				case EData_Type::CHAR_TYPE:
					emit_LIT(mToken_Value[0]);
					break;
				case EData_Type::BOOL_TYPE:
					emit_LIT(mToken_Value[0] == 't' ? 1 : 0);
					break;
				default:
					std::cout << "ERROR: unknown value type " << mToken_Value << std::endl;
					break;
			}
		};
};

#endif // __VALUE_NODE_H_