#ifndef __STATEMENT_NODE_H_
#define __STATEMENT_NODE_H_

#include <iostream>

#include "types.h"

class CStatement_Node
{
	public:
	
		virtual void Update_Break_Statements(unsigned int address)
		{
			//
		};

		virtual void Update_Continue_Statements(unsigned int address)
		{
			//
		};

		virtual void Update_Return_Type(EData_Type return_type)
		{
			//
		};

		virtual void Compile() = 0;
		
		virtual ~CStatement_Node() = default;
};

class CEmpty_Statement_Node : public CStatement_Node
{
	public:

		void Compile() override
		{
			std::cout << "CEmpty_Statement_Node::Compile()" << std::endl;

			// nothing to do here
		};
};

#endif // __STATEMENT_NODE_H_