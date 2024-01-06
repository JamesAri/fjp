#ifndef __BLOCK_NODE_H_
#define __BLOCK_NODE_H_

#include <vector>

#include "statement_node.h"
#include "break_node.h"
#include "continue_node.h"

#include "node_lists.h"

#include "pl0.h"
#include "generators.h"


class CBlock_Node : public CStatement_Node
{	
	private:
	
		statement_list_t *mStatement_List;

	public:

		CBlock_Node()
		{
			mStatement_List = new statement_list_t();
		};

		CBlock_Node(statement_list_t *statements)
		{
			this->mStatement_List = statements;
		};

		void Compile_Init()
		{
			std::cout << "CBlock_Node::Compile_Init()" << std::endl;

			sCurrent_Level = 0;
			sCurrent_Branch_Level = 0;
			sCode_Length = 0;
			sCurrent_Block_Address = 3;

			// for PL/0 control structures
			emit_INT(sCurrent_Block_Address);

			this->Compile();

			// terminate program
			emit_RET();
		};

		void Compile() override
		{
			std::cout << "CBlock_Node::Compile()" << std::endl;
			
			for (statement_list_t::iterator it = mStatement_List->begin(); it != mStatement_List->end(); ++it)
			{
				(*it)->Compile();
			}
		};

		void Update_Break_Statements(unsigned int address) override
		{
			std::cout << "CBlock_Node::Update_Break_Statements()" << std::endl;

			for (statement_list_t::iterator it = mStatement_List->begin(); it != mStatement_List->end(); ++it)
			{
				(*it)->Update_Break_Statements(address);
			}
		};

		void Update_Continue_Statements(unsigned int address) override
		{
			std::cout << "CBlock_Node::Update_Continue_Statements()" << std::endl;

			for (statement_list_t::iterator it = mStatement_List->begin(); it != mStatement_List->end(); ++it)
			{
				(*it)->Update_Continue_Statements(address);
			}
		};
};

#endif // __BLOCK_NODE_H_