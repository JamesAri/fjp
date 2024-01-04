#ifndef __LOG_H_
#define __LOG_H_

#include <iostream>
#include <iomanip>


#include "pl0.h"

// converts EIdentifier_Type to string
inline std::string identifier_type_to_string(EIdentifier_Type type)
{
	switch (type)
	{
		case EIdentifier_Type::VARIABLE:
			return "var";
		case EIdentifier_Type::PROCEDURE:
			return "procedure";
		default:
			return "unknown";
	}
}


// converts EData_Type to string
inline std::string data_type_to_string(EData_Type type)
{
	switch (type)
	{
		case EData_Type::INT_TYPE:
			return "int";
		case EData_Type::FLOAT_TYPE:
			return "float";
		case EData_Type::BOOL_TYPE:
			return "bool";
		case EData_Type::CHAR_TYPE:
			return "char";
		case EData_Type::VOID_TYPE:
			return "void";
		default:
			return "unknown";
	}
}


// prints the identifier table
inline void print_identifier_table()
{
	std::cout << "\nIdentifier Table:" << std::endl;
	std::cout << "-----------------" << std::endl;
	std::cout << std::left << std::setw(15) << "Name" << std::left << std::setw(15) << "Type" << std::left << std::setw(15) << "Data type" << std::left << std::setw(15) << "Address" << std::left << std::setw(15) << "Level" << std::left << std::setw(15) << "Branch level" << std::left << std::setw(15) << "Constant" << std::left << std::setw(15) << "Deleted" << std::endl;
	std::cout << std::left << std::setw(15) << "----" << std::left << std::setw(15) << "----" << std::left << std::setw(15) << "---------" << std::left << std::setw(15) << "-------" << std::left << std::setw(15) << "-----" << std::left << std::setw(15) << "------------" << std::left << std::setw(15) << "--------" << std::left << std::setw(15) << "-------" <<std::endl;
	for (int i = 0; i < sIdentifier_Count; i++)
	{
		std::cout 
		<< std::left << std::setw(15) << sIdentifier_Table[i].name
		<< std::left << std::setw(15) << identifier_type_to_string(sIdentifier_Table[i].type)
		<< std::left << std::setw(15) << data_type_to_string(sIdentifier_Table[i].data_type)
		<< std::left << std::setw(15) << sIdentifier_Table[i].address
		<< std::left << std::setw(15) << sIdentifier_Table[i].level
		<< std::left << std::setw(15) << sIdentifier_Table[i].branch_level
		<< std::left << std::setw(15) << (sIdentifier_Table[i].is_constant ? "true" : "false")
		<< std::left << std::setw(15) << (sIdentifier_Table[i].is_deleted ? "true" : "false")
		<< std::endl;
	}
	std::cout << std::endl;
}


#endif // __LOG_H_