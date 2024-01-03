#ifndef __STATEMENT_NODE_H_
#define __STATEMENT_NODE_H_

#include <iostream>

class CStatement_Node
{
	public:
		virtual void Compile() = 0;
};

#endif // __STATEMENT_NODE_H_