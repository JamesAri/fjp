#ifndef __BLOCK_NODE_H_
#define __BLOCK_NODE_H_

#include <vector>

#include "statement_node.h"

#include "pl0.h"
#include "generators.h"

typedef std::vector<CStatement_Node*> statement_list_t;

class CBlock_Node : public CStatement_Node
{	
	private:
	
		statement_list_t *mStatement_List;

	public:

		CBlock_Node() {};

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

			// Generete JMP at the beginning of the code
			// emit_JMP(0);

			// this->Compile();

			// Overwrite JMP with correct address
			// TCode_Entry_Value jmp_address;
			// jmp_address.i = -1;
			// sCode[0].param_2 = jmp_address;
		};

		void Compile() override
		{
			std::cout << "CBlock_Node::Compile()" << std::endl;
			
			for (statement_list_t::iterator it = mStatement_List->begin(); it != mStatement_List->end(); ++it)
			{
				(*it)->Compile();
			}
		};
};

#endif // __BLOCK_NODE_H_