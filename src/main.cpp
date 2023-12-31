#include <iostream>

#include "utils/definitions.h"

extern int yyparse();
extern FILE* yyin;

std::string sourceFile;

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
}

int main(int argc, char* argv[])
{	
	parse_argv(argc, argv);
	
	yyin = fopen(sourceFile.c_str(), "r");

	yyparse();

	fclose(yyin);

	return SUCCESS;
}