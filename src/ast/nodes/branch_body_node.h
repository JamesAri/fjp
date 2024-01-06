#ifndef __BRANCH_BODY_NODE_H__
#define __BRANCH_BODY_NODE_H__

#include "statement_node.h"
#include "block_node.h"

#include "pl0.h"

class CBranch_Body_Node : public CStatement_Node
{
	private:
		
		CStatement_Node *mStatement_Node;
		CBlock_Node *mBlock_Node;

	public:

		CBranch_Body_Node(CStatement_Node *statement) 
			: mStatement_Node(statement), mBlock_Node(nullptr)
		{
			//
		};

		CBranch_Body_Node(CBlock_Node *block) 
			: mStatement_Node(nullptr), mBlock_Node(block)
		{
			//
		};

		void Update_Break_Statements(unsigned int address) override
		{
			std::cout << "CBranch_Body_Node::Update_Break_Statements()" << std::endl;

			if(mStatement_Node != nullptr)
			{
				mStatement_Node->Update_Break_Statements(address);
			}
			else if(mBlock_Node != nullptr)
			{
				mBlock_Node->Update_Break_Statements(address);
			}
		};

		void Update_Continue_Statements(unsigned int address) override
		{
			std::cout << "CBranch_Body_Node::Update_Continue_Statements()" << std::endl;

			if (mStatement_Node != nullptr)
			{
				mStatement_Node->Update_Continue_Statements(address);
			}
			else if (mBlock_Node != nullptr)
			{
				mBlock_Node->Update_Continue_Statements(address);
			}
		};
		
		void Compile() override
		{
			std::cout << "CBranch_Body_Node::Compile()" << std::endl;

			if (mStatement_Node != nullptr)
			{
				mStatement_Node->Compile();
			}
			else if (mBlock_Node != nullptr)
			{
				mBlock_Node->Compile();
			}
		}
		
};

#endif // __BRANCH_BODY_NODE_H__