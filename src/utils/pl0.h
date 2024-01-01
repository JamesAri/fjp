#ifndef __PL0_H_
#define __PL0_H_

#define LIT   0
#define OPR   1
#define LOD   2
#define STO   3
#define CAL   4
#define RET   5
#define INT   6
#define JMP   7
#define JMC   8

#define NEG   1
#define ADD   2
#define SUB   3
#define MUL   4
#define DIV   5
#define MOD   6
#define ODD   7
#define EQ    8
#define NE    9
#define LT   10
#define GE   11
#define GT   12
#define LE   13

constexpr unsigned int Number_Of_Instructions = 9;

// 3 + terminating character
constexpr unsigned int Max_Instruction_Symbol_Size = 4;

typedef char instruction_symbol[Max_Instruction_Symbol_Size];

instruction_symbol Instruction_Symbol_Table[Number_Of_Instructions] = 
{"LIT", "OPR", "LOD", "STO", "CAL", "RET", "INT", "JMP", "JMC"};

#endif // __PL0_H_