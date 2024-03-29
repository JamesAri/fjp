#ifndef __FUNCTION_NODE_H_
#define __FUNCTION_NODE_H_

#include <iostream>

#include "log.h"

#include "statement_node.h"
#include "expression_node.h"
#include "identifier_node.h"
#include "declaration_node.h"
#include "block_node.h"

#include "node_lists.h"
#include "types.h"

#include "pl0.h"
#include "generators.h"

namespace
{
	void compile_function_parameter_list(declaration_list_t *parameter_list)
	{
		// increase SP by the size of arguments (parameters at this point)
		// they will be saved to identifer table below
		emit_INT(parameter_list->size());

		// declare function parameters and retreive the stored values (arguments) 
		// on top of the stack from the function call
		for (auto parameter : *parameter_list)
		{
			// Note: Do not compile the parameter here, because it will be dinamically allocated
			// since the function call already prepared the stack for the parameters, we just need to
			// add them to the identifier table.
			// parameter->Compile();

			add_identifier(
				parameter->Get_Identifier_Name().c_str(),
				EIdentifier_Type::VARIABLE,
				parameter->Get_Type(),
				sCurrent_Block_Address,
				sCurrent_Level,
				sCurrent_Branch_Level,
				parameter->Is_Constant()
			);

			sCurrent_Block_Address++;
		}
	}

	void compile_scope(CStatement_Node *function_body, declaration_list_t *parameter_list)
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

	void retrieve_value_from_return()
	{
		// 3 for control structuers of the previous function call and 1 for the return value = 4
		// since all our types are of size 1, this is fine for now, 
		// but with introduction of arrays, this will change
		emit_INT(4);
		// [SB, DB, PC, return_value] |

		// this pops the return value from the stack--> decreases SP by 1
		// this value is saved at the first address of the previous function call frame (control structure)
		emit_STO(0, sCurrent_Block_Address);
		// [return_value (SB), DB, PC] | [return_value]

		// go back over the previous function call control structures -> decrease SP by 3
		// but also increase SP by 1 because we want to retrieve the return value
		emit_INT(-3 + 1);
		// [return_value (SB)] | [DB, PC, return_value]
	}
}

class CReturn_Node : public CStatement_Node
{
	private:

		EData_Type mReturn_Type;
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

		void Update_Return_Type(EData_Type return_type) override
		{
			mReturn_Type = return_type;

			if (mExpression_Node == nullptr)
			{
				if (return_type != EData_Type::VOID_TYPE)
				{
					std::cerr << "ERROR: function has " << data_type_to_string(return_type) 
					<< " return type, but no return value was provided" << std::endl;
					exit(EXIT_FAILURE);
				}
			}
		};

		void Compile() override
		{
			std::cout << "CReturn_Node::Compile()" << std::endl;

			if (mExpression_Node)
			{
				if (mExpression_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
				{
					std::cerr << "ERROR: cannot return void type" << std::endl;
					exit(EXIT_FAILURE);
				}

				// push the value on top of the stack just after the control structures, 
				// function call will retrieve that value
				emit_INT(3 - sCurrent_Block_Address);

				mExpression_Node->Compile();

				// if variable has non-float type, but the expression has float type, round down to the nearest integer
				if (mExpression_Node->Get_Data_Type() == EData_Type::FLOAT_TYPE && mReturn_Type != EData_Type::FLOAT_TYPE)
				{
					emit_LIT(1);
					emit_OPR(PL0::Operations::DIV);
				}
			}

			emit_RET();
		};
};

class CFunction_Node : public CStatement_Node
{
	private:
	
		CType_Node *mReturn_Type_Node;
		CIdentifier_Node *mIdentifier_Node;
		declaration_list_t *mParameter_Declaration_List;

		CBlock_Node *mBody_Block_Node;

		function_parameter_list_t *mFunction_Parameter_List;


		void Create_Parameter_List()
		{
			mFunction_Parameter_List = new function_parameter_list_t();

			for (auto parameter : *mParameter_Declaration_List)
			{
				TFunction_Parameter fn_parameter;
				fn_parameter.data_type = parameter->Get_Type();
				fn_parameter.is_constant = parameter->Is_Constant();
				mFunction_Parameter_List->push_back(fn_parameter);
			}

		};
	
	public:

		CFunction_Node(CType_Node *return_type, CIdentifier_Node *identifier, declaration_list_t *parameters)
			: mReturn_Type_Node(return_type), mIdentifier_Node(identifier), mParameter_Declaration_List(parameters), mBody_Block_Node(nullptr)
		{
			Create_Parameter_List();
		};

		void Set_Body(CBlock_Node *body)
		{
			this->mBody_Block_Node = body;
		};

		void Compile() override
		{	
			std::cout << "CFunction_Node::Compile()" << std::endl;

			if (mBody_Block_Node == nullptr)
			{
				std::cerr << "ERROR: Function body not defined, function prototypes not allowed" << std::endl;
				exit(EXIT_FAILURE);
			}

			if (mReturn_Type_Node->mData_Type != EData_Type::VOID_TYPE && 
				dynamic_cast<CReturn_Node*>(mBody_Block_Node->mStatement_List->back()) == nullptr)
			{
				std::cerr << "ERROR: Function body must end with a return statement" << std::endl;
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
				mFunction_Parameter_List
			);
			
			mBody_Block_Node->Update_Return_Type(mReturn_Type_Node->mData_Type);

			compile_scope(mBody_Block_Node, mParameter_Declaration_List);

			// generate ret instruction if function has void return type and user doesn't
			// explicitly return from the function
			// this instruction will be ignored if user provided return statement
			if (mReturn_Type_Node->mData_Type == EData_Type::VOID_TYPE)
			{
				emit_RET();
			}
			
			// Overwrite JMP with correct address (after the fn definition)
			modify_param_2(jmp_instruction_address, sCode_Length);

		};
};

class CFunction_Call_Node : public CExpression_Node
{
	private:

		CIdentifier_Node *mIdentifier_Node;
		expression_list_t *mArgument_List;

	public:

		CFunction_Call_Node(CIdentifier_Node *identifier, expression_list_t *arguments)
		{
			this->mIdentifier_Node = identifier;
			this->mArgument_List = arguments;
		};

		void Compile() override
		{
			std::cout << "CFunction_Call_Node::Compile()" << std::endl;
			
			TIdentifier identifier = mIdentifier_Node->Find_Declared_Identifier(
				"ERROR: function not defined: " + mIdentifier_Node->mIdentifier
			);

			// we will update this expression node data type to the return type of the function
			mData_Type = identifier.data_type;

			if (identifier.type == EIdentifier_Type::VARIABLE)
			{
				std::cerr << "ERROR: you cannot call a variable: " << mIdentifier_Node->mIdentifier << std::endl;
				exit(EXIT_FAILURE);
			}

			if (mArgument_List->size() != identifier.parameter_list->size())
			{
				std::cerr << "ERROR: number of arguments does not match number of the function parameters: " << identifier.name << std::endl;
				exit(EXIT_FAILURE);
			}

			// First we want to compile the function arguments.
			// This will just push every statement value onto stack and
			// the function definition has instructions that will retrieve these
			// values.
		
			// - increase SP by 3 to skip over control structures when call is made
			emit_INT(3);

			// iterate over arguments and parameters and check if data types are compatible
			for(int i = 0; i < mArgument_List->size(); i++)
			{
				CExpression_Node *argument = (*mArgument_List)[i];
				TFunction_Parameter parameter = (*identifier.parameter_list)[i];

				argument->Compile();

				// check if argument is not void
				if (argument->Get_Data_Type() == EData_Type::VOID_TYPE)
				{
					std::cerr << "ERROR: cannot pass 'void' type as an argument" << std::endl;
					exit(EXIT_FAILURE);
				}

				// if parameter has non-float type, but the argument is of float type, round down to the nearest integer
				if (argument->Get_Data_Type() == EData_Type::FLOAT_TYPE && parameter.data_type != EData_Type::FLOAT_TYPE)
				{
					emit_LIT(1);
					emit_OPR(PL0::Operations::DIV);
				}
			}
			
			// Return back where we were:
			// - decrease SP by 3 to skip over control structures
			// - decrease SP by argument list size to skip over generated expression 
			// 	 values on the stack
			emit_INT(-(3 + mArgument_List->size()));

			// Emit function call instruction:
			emit_CAL(sCurrent_Level - identifier.level, identifier.address);

			// Now if this function has non-void type, we want to retrieve the 
			// return value from the previous function call frame and push it on the stack:

			if (identifier.data_type != EData_Type::VOID_TYPE)
			{
				retrieve_value_from_return();
			}
		};
};


#endif // __FUNCTION_NODE_H_