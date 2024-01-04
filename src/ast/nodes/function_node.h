#ifndef __FUNCTION_NODE_H_
#define __FUNCTION_NODE_H_

#include <iostream>

#include "statement_node.h"
#include "expression_node.h"

#include "types.h"

#include "pl0.h"
#include "generators.h"

typedef std::vector<CDeclaration_Node*> parameter_list_t;
typedef std::vector<CExpression_Node*> argument_list_t;

namespace
{
	void compile_function_parameter_list(parameter_list_t *parameter_list)
	{
		// declare function parameters
		for (auto parameter : *parameter_list)
		{
			parameter->Compile();

			std::string name = parameter->Get_Identifier_Name();

			int index = find_identifier(name.c_str());

			if (index == FAILURE)
			{
				std::cout << "ERROR: compiler error, identifier not found but should have been found: " << name << std::endl;
				exit(EXIT_FAILURE);
			}

			const TIdentifier identifier = sIdentifier_Table[index];

			// when function is called, value for the declared function parameters 
			// will be on top of the stack, so we retrieve them one by one
			emit_STO(sCurrent_Level - identifier.level, identifier.address);
		}
	}

	void compile_scope(CStatement_Node *function_body, parameter_list_t *parameter_list)
	{
		sCurrent_Level++;

		unsigned int saved_current_block_address = sCurrent_Block_Address;
		
		sCurrent_Block_Address = 3;

		// for PL/0 control structures
		emit_INT(sCurrent_Block_Address);

		// first compile function parameters
		compile_function_parameter_list(parameter_list);

		// then function body
		function_body->Compile();

		// restore block address
		sCurrent_Block_Address = saved_current_block_address;

		// mark all identifiers in current scope as deleted
		mark_scope_identifiers_as_deleted(sCurrent_Level);
			
		sCurrent_Level--;
	}
}

class CFunction_Node : public CStatement_Node
{
	private:
	
		CType_Node *mReturn_Type_Node;
		CIdentifier_Node *mIdentifier_Node;
		parameter_list_t *mParameter_List;

		CBlock_Node *mBody_Block_Node;
	
	public:

		CFunction_Node(CType_Node *return_type, CIdentifier_Node *identifier, parameter_list_t *parameters)
			: mReturn_Type_Node(return_type), mIdentifier_Node(identifier), mParameter_List(parameters), mBody_Block_Node(nullptr)
		{
			//
		};

		void Set_Body(CBlock_Node *body)
		{
			this->mBody_Block_Node = body;
		};

		void Compile() override
		{	
			std::cout << "CFunction_Node::Compile()" << std::endl;

			if (!mBody_Block_Node)
			{
				std::cout << "ERROR: Function body not defined, function prototypes not allowed" << std::endl;
				exit(EXIT_FAILURE);
			}


			// Save the address of the JMP instruction below
			int jmp_instruction_address = sCode_Length;
			// To be overwritten later
			emit_JMP(0);

			add_identifier(
				mIdentifier_Node->mIdentifier.c_str(), 
				EIdentifier_Type::PROCEDURE,
				mReturn_Type_Node->mData_Type, 
				sCode_Length, // this now points after the JMP instruction above - to the beginning of the function body
				sCurrent_Level,
				sCurrent_Branch_Level,
				false,
				mParameter_List->size()
			);
			
			compile_scope(mBody_Block_Node, mParameter_List);
			
			// Overwrite JMP with correct address (after the fn definition)
			modify_param_2(jmp_instruction_address, sCode_Length);
		};
};

class CFunction_Call_Node : public CExpression_Node
{
	private:

		CIdentifier_Node *mIdentifier_Node;
		argument_list_t *mArgument_List;

	public:

		CFunction_Call_Node(CIdentifier_Node *identifier, argument_list_t *arguments)
		{
			this->mIdentifier_Node = identifier;
			this->mArgument_List = arguments;
		};

		void Compile() override
		{
			std::cout << "CFunction_Call_Node::Compile()" << std::endl;
			
			// check if identifier is declared
			int index = find_identifier(mIdentifier_Node->mIdentifier.c_str());

			if (index == FAILURE)
			{
				std::cout << "ERROR: function not defined: " << mIdentifier_Node->mIdentifier << std::endl;
				exit(EXIT_FAILURE);
			}

			TIdentifier identifier = sIdentifier_Table[index];

			// check if identifier is not a variable
			if (identifier.type == EIdentifier_Type::VARIABLE)
			{
				std::cout << "ERROR: you cannot call a variable: " << mIdentifier_Node->mIdentifier << std::endl;
				exit(EXIT_FAILURE);
			}

			// check if number of arguments matches number of parameters
			if (mArgument_List->size() != identifier.number_of_parameters)
			{
				std::cout << "ERROR: number of arguments does not match number of the function parameters: " << identifier.name << std::endl;
				exit(EXIT_FAILURE);
			}

			// Compile arguments:
		
			// 3 to skip over control structures when call is made
			emit_INT(3);

			for (auto argument : *mArgument_List)
			{
				argument->Compile();
			}

			// Return back where we were -> 3 from skipping control structures and mArgument_List->size()
			// from generated experssions on the stack.
			// When call is made, function body will pop the arguments.
			emit_INT(-(3 + mArgument_List->size()));

			// Emit function call instruction:
			emit_CAL(sCurrent_Level - identifier.level, identifier.address);
		};
};

class CReturn_Node : public CStatement_Node
{
	private:

		CExpression_Node *mExpression_Node;

	public:

		CReturn_Node() : mExpression_Node(nullptr)
		{
			//
		};

		CReturn_Node(CExpression_Node *expression) : mExpression_Node(expression)
		{
			//
		};

		void Compile() override
		{
			std::cout << "CReturn_Node::Compile()" << std::endl;

			if (mExpression_Node)
			{
				// check if return value isn't void type

				if (mExpression_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
				{
					std::cout << "ERROR: cannot return void type" << std::endl;
					exit(EXIT_FAILURE);
				}
				// TODO: move correctly stack
				mExpression_Node->Compile();
			}

			emit_RET();
		};
};


#endif // __FUNCTION_NODE_H_