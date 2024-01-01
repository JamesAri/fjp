#ifndef __EXPRESSION_NODE_H_
#define __EXPRESSION_NODE_H_

#include <iostream>

#include "inode.h"


class CExpressionNode : public INode
{
	public:

		CExpressionNode()
		{
			std::cout << "CExpressionNode constructor" << std::endl;
		};

		void Compile()
		{
			// Do nothing
		};
};

#endif // __EXPRESSION_NODE_H_