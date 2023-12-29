#!/bin/bash

win_build() {
	echo "Windows build"
	# Bison
	bison -dy grammar.y
	# Flex
	flex lexer.l
	# GCC
	gcc lex.yy.c y.tab.c -o prog.exe
}

if [ "$(uname)" == "Darwin" ]; then
	# Mac OS X platform
	echo "TODO: Mac OS X build"
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
