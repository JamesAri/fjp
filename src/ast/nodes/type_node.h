#ifndef __TYPE_NODE_H_
#define __TYPE_NODE_H_

#include <iostream>

#include "types.h"


class CType_Node
{
	public:

		const EData_Type mData_Type;

		CType_Node(const EData_Type &type) : mData_Type(type)
		{
			//
		};
};

#endif // __TYPE_NODE_H_