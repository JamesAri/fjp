#include <iostream>
#include <fstream>

#include "common.h"
#include "ast/nodes/block_node.h"

#include "log.h"

// Bison specific prototypes
extern int yyparse();
extern int yydebug;
extern FILE* yyin;

// Root node of the AST - created in scanner
extern CBlock_Node* sRootNode;

// Source file to be parsed
std::string sourceFile;
// Output file
std::string outputFile = "out.pl0";

// Enables bison debug messages
void enable_bison_debug_messages()
{
	#ifdef YYDEBUG
    yydebug = 1;
    #endif
}

void parse_argv(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: " << argv[0] << " <input file>" << std::endl;
		exit(EXIT_FAILURE);
	}

	sourceFile = argv[1];

	// check if file exists
	FILE* file = fopen(sourceFile.c_str(), "r");

	if (file == NULL)
	{
		std::cout << "File " << sourceFile << " does not exist." << std::endl;
		exit(EXIT_FAILURE);
	}
	{
		fclose(file);
	}

	for(int i = 2; i < argc; i++)
	{	
		// -d flag to enable bison debug messages
		if (strcmp(argv[i], "-d") == 0)
		{
			enable_bison_debug_messages();
		}
		// -o flag to specify output file
		else if (strcmp(argv[i], "-o") == 0)
		{
			if (i + 1 < argc)
			{
				outputFile = argv[i + 1];
			}
			else
			{
				std::cout << "Usage: " << argv[0] << " <input file> [-d] [-o <output file>]" << std::endl;
				exit(EXIT_FAILURE);
			}
		}
	}
}

// outputs the code to a file
void output_code()
{
	std::ofstream file;
	file.open(outputFile);

	if (!file.is_open())
	{
		std::cout << "ERROR: invalid path " << outputFile << std::endl;
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < sCode_Length; i++)
	{
		const TCode_Entry entry = sCode[i];
		const std::string instruction_symbol = Instruction_Symbol_Table[entry.instruction];
		const int param1 = entry.param_1;
		auto param2 =  entry.is_float ? entry.param_2.f : entry.param_2.i;
		file << instruction_symbol << " " << param1 << " " << param2 << std::endl;
	}

	file.close();
}

int main(int argc, char* argv[])
{	
	parse_argv(argc, argv);
	
	yyin = fopen(sourceFile.c_str(), "r");

	int res = yyparse();

	fclose(yyin);

	if(res != 0)
	{
		std::cout << "ERROR: Parsing failed" << std::endl;
		exit(EXIT_FAILURE);
	}

	sRootNode->Compile_Init();
	
	output_code();

	print_identifier_table();

	return EXIT_SUCCESS;
}