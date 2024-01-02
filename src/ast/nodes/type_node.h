#ifndef __TYPE_NODE_H_
#define __TYPE_NODE_H_

#include <iostream>

#include "types.h"


class CType_Node
{
	private:
		EData_Type type;

	public:


		CType_Node(const EData_Type &type) : type(type)
		{
			//
		};

		EData_Type Get_Type()
		{
			return type;
		};
};

#endif // __TYPE_NODE_H_