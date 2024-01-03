#ifndef __TYPE_NODE_H_
#define __TYPE_NODE_H_

#include <iostream>

#include "types.h"


class CType_Node
{
	private:
		EData_Type mType;

	public:


		CType_Node(const EData_Type &type) : mType(type)
		{
			//
		};

		EData_Type Get_Type()
		{
			return mType;
		};
};

#endif // __TYPE_NODE_H_