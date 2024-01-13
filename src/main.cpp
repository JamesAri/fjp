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
extern CBlock_Node* sRoot_Node;

// Source file to be parsed - defined in common, so we can use it in the scanner error function
std::string sSource_File;
// Output file
std::string output_file = "out.pl0";

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

	sSource_File = argv[1];

	// check if file exists
	FILE* file = fopen(sSource_File.c_str(), "r");

	if (file == NULL)
	{
		std::cout << "File " << sSource_File << " does not exist." << std::endl;
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
				output_file = argv[i + 1];
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
	file.open(output_file);

	if (!file.is_open())
	{
		std::cerr << "ERROR: invalid path " << output_file << std::endl;
		exit(EXIT_FAILURE);
	}

	for (int i = 0; i < sCode_Length; i++)
	{
		const TCode_Entry entry = sCode[i];
		const std::string instruction_symbol = Instruction_Symbol_Table[entry.instruction];
		const int param1 = entry.param_1;
		auto param2 =  entry.is_float ? entry.param_2.f : entry.param_2.i;
		file << i << " " << instruction_symbol << " " << param1 << " " << param2 << std::endl;
	}

	file.close();
}

int main(int argc, char* argv[])
{	
	parse_argv(argc, argv);
	
	yyin = fopen(sSource_File.c_str(), "r");

	int res = yyparse();

	fclose(yyin);

	if(res != 0)
	{
		std::cerr << "error: syntax parsing failed due to unrecognized token" << std::endl;
		exit(EXIT_FAILURE);
	}

	sRoot_Node->Compile_Init();
	
	output_code();

	print_identifier_table();

	return EXIT_SUCCESS;
}