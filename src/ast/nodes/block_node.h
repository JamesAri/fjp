#ifndef __BLOCK_NODE_H_
#define __BLOCK_NODE_H_

#include <vector>

#include "statement_node.h"

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
			// for (int i = 0; i < statements->size(); ++i)
			// {
			// 	std::cout << "CBlock_Node::Compile()" << std::endl;
			// 	std::cout << "statements->size() = " << statements->size() << std::endl;
			// 	std::cout << "i = " << i << std::endl;
			// 	(*statements)[i]->Compile();
			// }

			std::cout << "CBlock_Node::Compile()" << std::endl;
			
			for (statement_list_t::iterator it = statements->begin(); it != statements->end(); ++it)
			{
				(*it)->Compile();
			}
		};
};

#endif // __BLOCK_NODE_H_