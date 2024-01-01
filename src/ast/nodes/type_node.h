#ifndef __TYPE_NODE_H_
#define __TYPE_NODE_H_

#include <iostream>

#include "inode.h"

class CTypeNode : public INode
{
	public:

		CTypeNode()
		{
			std::cout << "CTypeNode constructor" << std::endl;
		};

		void Compile()
		{
			// Do nothing
		};
};

#endif // __TYPE_NODE_H_