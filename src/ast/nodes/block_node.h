#ifndef __BLOCK_NODE_H_
#define __BLOCK_NODE_H_

#include <vector>

#include "statement_node.h"

#include "pl0.h"

typedef std::vector<CStatement_Node*> statement_list_t;

class CBlock_Node : public CStatement_Node
{	
	private:
		statement_list_t *statements;

	public:

		CBlock_Node() {};

		CBlock_Node(statement_list_t *statements)
		{
			this->statements = statements;
		};

		void Compile() override
		{
			std::cout << "CBlock_Node::Compile()" << std::endl;
			
			sCurrent_Level++;
			for (statement_list_t::iterator it = statements->begin(); it != statements->end(); ++it)
			{
				(*it)->Compile();
			}
			sCurrent_Level--;
		};
};

#endif // __BLOCK_NODE_H_