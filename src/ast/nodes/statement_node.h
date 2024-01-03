#ifndef __STATEMENT_NODE_H_
#define __STATEMENT_NODE_H_

#include <iostream>

class CStatement_Node
{
	public:
		virtual void Compile() = 0;
		virtual ~CStatement_Node() = default;
};

#endif // __STATEMENT_NODE_H_