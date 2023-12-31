#ifndef __STATEMENT_NODE_H_
#define __STATEMENT_NODE_H_

#include <iostream>

class CStatementNode 
{
	public:

		CStatementNode()
		{
			std::cout << "CStatementNode constructor" << std::endl;
		};

		void Compile()
		{
			// Do nothing
		};
};

#endif // __STATEMENT_NODE_H_