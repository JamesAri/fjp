#!/bin/bash

mkdir -p build >/dev/null 2>&1
cd build

# path to lex file
lex_file="../src/rules/lex.l"

# path to grammar file
grammar_file="../src/rules/grammar.y"

# gcc definitions
gcc_files="-I ../src/nodes ../src/main.cpp lex.yy.cpp y.tab.cpp"

# -DYYDEBUG=1 for bison verbose messages
gcc_flags="-std=c++11 -DYYDEBUG=1"

win_build() {
	echo "Windows build"
	# Bison
	bison -d $grammar_file -o y.tab.cpp
	# Flex
	flex -o lex.yy.cpp $lex_file
	# GCC
	g++ $gcc_flags $gcc_files -o compiler.exe
}

osx_build() {
	echo "Mac OS X build"
	# Bison
	bison -d $grammar_file -o y.tab.cpp
	# Flex
	flex -o lex.yy.cpp $lex_file
	# GCC
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
