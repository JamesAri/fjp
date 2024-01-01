#include <iostream>

#include "definitions.h"
#include "ast/inode.h"

// Bison specific prototypes
extern int yyparse();
extern int yydebug;
extern FILE* yyin;

// Root node of the AST - created in scanner
extern INode* rootNode;

// Source file to be parsed
std::string sourceFile;

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
		exit(FAILURE);
	}

	sourceFile = argv[1];

	// check if file exists
	FILE* file = fopen(sourceFile.c_str(), "r");

	if (file == NULL)
	{
		std::cout << "File " << sourceFile << " does not exist." << std::endl;
		exit(FAILURE);
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
	}
}

int main(int argc, char* argv[])
{	
	parse_argv(argc, argv);
	
	yyin = fopen(sourceFile.c_str(), "r");

	yyparse();

	fclose(yyin);

	return SUCCESS;
}