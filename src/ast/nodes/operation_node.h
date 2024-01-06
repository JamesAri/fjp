#ifndef __OPERATION_NODE_H_
#define __OPERATION_NODE_H_

#include <iostream>

#include "log.h"
#include "common.h"

#include "statement_node.h"
#include "expression_node.h"

#include "operators.h"
#include "types.h"

#include "pl0.h"
#include "generators.h"

class CUnary_Operation_Node : public CExpression_Node
{
	private:
		
		CExpression_Node *mExpression_Node;
		EUnary_Operation mOperation;

	public:

		CUnary_Operation_Node(CExpression_Node *expression, const EUnary_Operation &operation)
			: CExpression_Node(expression->Get_Data_Type(), true), mExpression_Node(expression), mOperation(operation)
		{
			//
		}

		void Update_Break_Statements(unsigned int address) override
		{
			//
		};

		void Compile() override
		{
			std::cout << "CUnary_Operation_Node::Compile()" << std::endl;

			mExpression_Node->Compile();

			switch (mOperation)
			{
				case EUnary_Operation::MINUS:
					emit_OPR(PL0::Operations::NEG);
					break;
				case EUnary_Operation::PLUS:
					// do nothing, correct value is already on top of the stack
					break;
				case EUnary_Operation::NOT:
					emit_LIT(0);
					emit_OPR(PL0::Operations::EQ);
					break;
			}
		}
};

// Base class for all binary operations
class CBinary_Operation_Node : public CExpression_Node
{
	protected:
		
		CExpression_Node *mLhs_Node;
		CExpression_Node *mRhs_Node;

	public:

		// data type will be known when both expressions are compiled
		CBinary_Operation_Node(CExpression_Node *lhs, CExpression_Node *rhs)
			: CExpression_Node(EData_Type::UNKNOWN, true), mLhs_Node(lhs), mRhs_Node(rhs)
		{
			// 
		}

		virtual void Compile() override = 0;
};

class CArithmetic_Operation_Node : public CBinary_Operation_Node
{
	private:

		EArithmetic_Operation mOperation;

	public:

		CArithmetic_Operation_Node(CExpression_Node *lhs, CExpression_Node *rhs, const EArithmetic_Operation &operation)
			: CBinary_Operation_Node(lhs, rhs), mOperation(operation)
		{
			//
		}

		virtual void Compile() override
		{
			std::cout << "CArithmetic_Operation_Node::Compile()" << std::endl;

			mLhs_Node->Compile();
			mRhs_Node->Compile();

			if (mLhs_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: cannot perform arithmetic operations on 'void' type" << std::endl;
				exit(EXIT_FAILURE);
			}

			// check if both expressions have compatible data types
			if (mLhs_Node->Get_Data_Type() != mRhs_Node->Get_Data_Type())
			{
				std::cerr << "ERROR: incompatible data types in binary operation" << std::endl;
				std::cout << "Left hand side data type: " << data_type_to_string(mLhs_Node->Get_Data_Type()) << std::endl;
				std::cout << "Right hand side data type: " << data_type_to_string(mRhs_Node->Get_Data_Type()) << std::endl;
				exit(EXIT_FAILURE);
			}

			// we take over the data type of the left hand side expression
			this->mData_Type = mLhs_Node->Get_Data_Type();

			switch (mOperation)
			{
				case EArithmetic_Operation::ADD:
					emit_OPR(PL0::Operations::ADD);
					break;
				case EArithmetic_Operation::SUB:
					emit_OPR(PL0::Operations::SUB);
					break;
				case EArithmetic_Operation::MUL:
					emit_OPR(PL0::Operations::MUL);
					break;
				case EArithmetic_Operation::DIV:
					emit_OPR(PL0::Operations::DIV);
					break;
				case EArithmetic_Operation::MOD:
					emit_OPR(PL0::Operations::MOD);
					break;
			}
		}
};

class CLogical_Operation_Node : public CBinary_Operation_Node
{
	private:

		ELogical_Operation mOperation;

	public:

		CLogical_Operation_Node(CExpression_Node *lhs, CExpression_Node *rhs, const ELogical_Operation &operation)
			: CBinary_Operation_Node(lhs, rhs), mOperation(operation)
		{
			//
		}

		virtual void Compile() override
		{
			std::cout << "CLogical_Operation_Node::Compile()" << std::endl;

			if (mLhs_Node->Get_Data_Type() == EData_Type::VOID_TYPE || mRhs_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: logical operation failed, could not convert from 'void' to 'bool'" << std::endl;
				exit(EXIT_FAILURE);
			}

			int lhs_jmc_instruction_address, rhs_jmp_instruction_address;

			switch (mOperation)
			{
				case ELogical_Operation::AND:
					mLhs_Node->Compile();
					emit_LIT(0);
					emit_OPR(PL0::Operations::NE);

					// jump over the second statement execution if the first statement was false (zero)
					// So if value on top of the stack is:
					// - true: skip over the second statement (JMP) because first was false
					// - false: execute the second statement because first was true
					lhs_jmc_instruction_address = sCode_Length;
					emit_JMC(0);
					
					mRhs_Node->Compile();

					rhs_jmp_instruction_address = sCode_Length;
					emit_JMP(0);

					modify_param_2(lhs_jmc_instruction_address, sCode_Length);

					// JMC moved the SP by 1, so we need to move it back by 1 to retrieve the boolean value (which is false)
					// emit_INT(1);
					emit_LIT(0);
					
					// jump over the INT emit, since the value is already on the stack from the second statement
					modify_param_2(rhs_jmp_instruction_address, sCode_Length);

					break;

				case ELogical_Operation::OR:
					mLhs_Node->Compile();
					emit_LIT(0);
					emit_OPR(PL0::Operations::EQ);

					// at this point, if the first statement was false (zero), there is true (non-zero) value on top of the stack
					// if first statement was true, we want to jump over the second statement execution
					// So if value on top of the stack is: 	
					// - true: execute the second statement because first was false
					// - false: skip over the second statement (JMC) because first was true
					lhs_jmc_instruction_address = sCode_Length;
					emit_JMC(0);

					mRhs_Node->Compile();

					rhs_jmp_instruction_address = sCode_Length;
					emit_JMP(0);

					modify_param_2(lhs_jmc_instruction_address, sCode_Length);

					// JMC moved the SP by 1, so we need to move it back by 1 to retrieve the boolean value (which is true)
					// emit_INT(1);
					emit_LIT(1);
					
					// jump over the INT emit, since the value is already on the stack from the second statement
					modify_param_2(rhs_jmp_instruction_address, sCode_Length);

					break;
			}

		}
};

class CRelational_Operation_Node : public CBinary_Operation_Node
{
	private:

		ERelational_Operation mOperation;

	public:

		CRelational_Operation_Node(CExpression_Node *lhs, CExpression_Node *rhs, const ERelational_Operation &operation)
			: CBinary_Operation_Node(lhs, rhs), mOperation(operation)
		{
			//
		}

		virtual void Compile() override
		{
			std::cout << "CRelational_Operation_Node::Compile()" << std::endl;

			mLhs_Node->Compile();

			mRhs_Node->Compile();

			if (mLhs_Node->Get_Data_Type() == EData_Type::VOID_TYPE || mRhs_Node->Get_Data_Type() == EData_Type::VOID_TYPE)
			{
				std::cerr << "ERROR: compare operation failed, could not convert from 'void' to 'bool'" << std::endl;
				exit(EXIT_FAILURE);
			}

			switch (mOperation)
			{
				case ERelational_Operation::EQUAL:
					emit_OPR(PL0::Operations::EQ);
					break;
				case ERelational_Operation::NOT_EQUAL:
					emit_OPR(PL0::Operations::NE);
					break;
				case ERelational_Operation::LESS_THAN:
					emit_OPR(PL0::Operations::LT);
					break;
				case ERelational_Operation::LESS_THAN_EQUAL:
					emit_OPR(PL0::Operations::LE);
					break;
				case ERelational_Operation::GREATER_THAN:
					emit_OPR(PL0::Operations::GT);
					break;
				case ERelational_Operation::GREATER_THAN_EQUAL:
					emit_OPR(PL0::Operations::GE);
					break;
			}
		}
};


#endif // __OPERATION_NODE_H_