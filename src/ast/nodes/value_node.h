#ifndef __VALUE_NODE_H_
#define __VALUE_NODE_H_

#include <iostream>

#include "inode.h"
#include "tokens.h"


class CValueNode : public INode
{
	public:
		TTokenValue tokenValue;

		CValueNode(TTokenValue tokenValue)
		{
			std::cout << "CValueNode constructor" << std::endl;
			this->tokenValue = tokenValue;
		};

		void Compile()
		{
			// Do nothing
		};
};

#endif // __VALUE_NODE_H_