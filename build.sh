#!/bin/bash

mkdir -p build >/dev/null 2>&1
cd build

# path to lex file
lex_file="../src/rules/lex.l"

# path to grammar file
grammar_file="../src/rules/grammar.y"

# gcc definitions
gcc_files="-I ../src/ast -I ../src/utils y.tab.cpp lex.yy.cpp ../src/main.cpp"

# -DYYDEBUG=1 for bison verbose messages
gcc_flags="-std=gnu++11 -DYYDEBUG=1"

if [ "$1" == "debug" ]; then
    gcc_flags+=" -g -Wall"
fi

flex_bison_build()
{
	# Bison
	bison -o y.tab.cpp -d $grammar_file 
	# Flex
	flex -o lex.yy.cpp $lex_file
}

win_build() 
{
	echo "Windows build"
	flex_bison_build
	g++ $gcc_flags $gcc_files -o compiler.exe
}

osx_build() 
{
	echo "Mac OS X build"
	flex_bison_build
	g++ $gcc_flags $gcc_files -o compiler -ll
}

if [ "$(uname)" == "Darwin" ]; then
	# Mac OS X platform
	osx_build
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
	# GNU/Linux platform
	echo "TODO: GNU/Liux build"
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
	# 32 bits Windows NT platform
	win_build
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
	# 64 bits Windows NT platform
	win_build
fi

# Wait for enter
read -p "Press enter to continue"
