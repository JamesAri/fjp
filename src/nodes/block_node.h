#ifndef __BLOCK_NODE_H_
#define __BLOCK_NODE_H_

#include "statement_node.h"

class CBlockNode : public INode
{
	public:

		CBlockNode()
		{
			std::cout << "CBlockNode constructor" << std::endl;
		};

		void AddStatement(CStatementNode *statement)
		{
			// Do nothing
		};

		void Compile()
		{
			std::cout << "Compiling" << std::endl;
		};
};

#endif // __BLOCK_NODE_H_