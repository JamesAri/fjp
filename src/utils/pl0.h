#ifndef __PL0_H_
#define __PL0_H_

#include <stack>

#include "types.h"
#include "common.h"

/*
Basic ISA for PL/0

lit 0,A    		ulož konstantu A do zásobníku
opr 0,A    		proveď instrukci A z tabulky níže (vysledek na vrcholu zásobníku)
1    unární minus
2    +
3    -
4    *
5    div - celočíselné dělení (znak /)
6    mod - dělení modulo (znak %)
7    odd - test, zda je číslo liché
8    test rovnosti (znak =)
9    test nerovnosti (znaky <>)
10    <
11    >=
12    >
13    <=
lod L,A    		ulož hodnotu proměnné z adr. L,A na vrchol zásobníku
sto L,A    		zapiš do proměnné z adr. L,A hodnotu z vrcholu zásobníku
cal L,A    		volej proceduru A z úrovně L
int 0,A   		zvyš obsah top-registru zásobníku o hodnotu A
jmp 0,A    		proveď skok na adresu A
jmc 0,A    		proveď skok na adresu A, je-li hodnota na vrcholu zásobníku 0
ret 0,0    		návrat z procedury (return)

Extension:

REA 0 0        na vrchol zasobniku nactene cislo (read)
WRI 0 0        vypis ascii znaku podle vrcholu zasobniku (write)
OPF 0 funkce   modifikace zasobniku s plovoucimi hodnotami
RTI 0 0        ze 2 hodnot (double) na zasobniku se udela 1 (int)
ITR 0 0        z 1 hodnoty (int) na zasobniku se udelaji 2 (double)
NEW 0 0        alokace 1 bunky pameti, na zasobnik jeji adresa
DEL 0 0        dealokace 1 bunky pameti, jeji adresa na vrcholu zasobniku
LDA 0 0		   na vrchol zasobniku data z adresy uvedene na vrcholu zasobniku
STA 0 0        na zasobniku hodnota, adresa -> ulozeni hodnoty na adresu
PLD 0 0        na zasobniku L, A -> na vrchol hodnota z (L, A)
PST 0 0        na zasobnkku X, L, A -> na adresu (L, A) se ulozi X

*/

typedef unsigned int instruction_t;
typedef unsigned int operation_t;

// Description of the extended PL/0 instruction set (help tab): https://home.zcu.cz/~lipka/fjp/pl0/ 
namespace PL0
{
	constexpr instruction_t LIT =  0;
	constexpr instruction_t OPR =  1;
	constexpr instruction_t LOD =  2;
	constexpr instruction_t STO =  3;
	constexpr instruction_t CAL =  4;
	constexpr instruction_t RET =  5;
	constexpr instruction_t INT =  6;
	constexpr instruction_t JMP =  7;
	constexpr instruction_t JMC =  8;
	constexpr instruction_t REA =  9;     
	constexpr instruction_t WRI = 10;     
	constexpr instruction_t OPF = 11;     // not supported by web interpreter
	constexpr instruction_t RTI = 12;     // not supported by web interpreter
	constexpr instruction_t ITR = 13;     // not supported by web interpreter
	constexpr instruction_t NEW = 14;     
	constexpr instruction_t DEL = 15;     
	constexpr instruction_t LDA = 16;	
	constexpr instruction_t STA = 17;   
	constexpr instruction_t PLD = 18;
	constexpr instruction_t PST = 19;

	constexpr operation_t NEG =  1;
	constexpr operation_t ADD =  2;
	constexpr operation_t SUB =  3;
	constexpr operation_t MUL =  4;
	constexpr operation_t DIV =  5;
	constexpr operation_t MOD =  6;
	constexpr operation_t ODD =  7;
	constexpr operation_t EQ  =  8;
	constexpr operation_t NE  =  9;
	constexpr operation_t LT  = 10;
	constexpr operation_t GE  = 11;
	constexpr operation_t GT  = 12;
	constexpr operation_t LE  = 13;
}


constexpr unsigned int Number_Of_Instructions = 20;

constexpr unsigned int Max_Instruction_Symbol_Size = 3;

// +1 for terminating character
typedef char instruction_symbol_t[Max_Instruction_Symbol_Size + 1];

constexpr instruction_symbol_t Instruction_Symbol_Table[Number_Of_Instructions] = 
{"LIT", "OPR", "LOD", "STO", "CAL", "RET", "INT", "JMP", "JMC", "REA", "WRI", "OPF", "RTI", "ITR", "NEW", "DEL", "LDA", "STA", "PLD", "PST"};


// CODE STORAGE
constexpr unsigned int Max_Code_Length = 10000;

typedef int code_t[Max_Code_Length][3]; // <instruction> <param_1> <param_2>

extern code_t sCode;
extern int sCurrent_Level;
extern unsigned int sCode_Length;
extern unsigned int sCurrent_Block_Address;


// IDENTIFIER STORAGE
constexpr unsigned int Max_Identifier_Table_Length = 500;

typedef TIdentifier identifier_table_t[Max_Identifier_Table_Length];

extern identifier_table_t sIdentifier_Table;
extern unsigned int sIdentifier_Count;


// GLOBAL STACK
extern std::stack<int> sStack;


// UTILITY FUNCTIONS
inline void emit(const instruction_t instruction, const int param_1, const int param_2)
{
	sCode[sCode_Length][0] = instruction;
	sCode[sCode_Length][1] = param_1;
	sCode[sCode_Length][2] = param_2;
	sCode_Length++;
}

inline int find_identifier(const char *name, const unsigned int level = 0)
{
	// we want the most recent identifier
	for (int i = sIdentifier_Count - 1; i >= 0; i--)
	{
		if (sIdentifier_Table[i].level < level)
		{
			return FAILURE;
		}
		if (strcmp(sIdentifier_Table[i].name, name) == 0)
		{
			return i;
		}
	}
	
	return FAILURE;
}

inline void add_identifier(const char *name, const EIdentifier_Type type, EData_Type data_type, const unsigned int address, const unsigned int level, const bool is_constant)
{	
	if (sIdentifier_Count >= Max_Identifier_Table_Length)
	{
		std::cout << "ERROR: Identifier table overflow" << std::endl;
		exit(EXIT_FAILURE);
	}

	if (find_identifier(name) != FAILURE)
	{
		std::cout << "ERROR: Identifier already exists" << std::endl;
		exit(EXIT_FAILURE);
	}

	TIdentifier *identifier = &sIdentifier_Table[sIdentifier_Count];
	
	identifier->type = type;
	identifier->data_type = data_type;
	identifier->address = address;
	identifier->level = level;
	identifier->is_constant = is_constant;
	
	for (unsigned int i = 0; i < Max_Identifier_Length; i++)
	{
		identifier->name[i] = name[i];
		
		if (name[i] == '\0')
		{
			break;
		}
	}
	identifier->name[Max_Identifier_Length] = '\0';
	
	sIdentifier_Count++;
}

inline void emit_LIT(const int value)
{
	emit(PL0::LIT, 0, value);
}

inline void emit_OPR(const operation_t operation)
{
	emit(PL0::OPR, 0, operation);
}

inline void emit_LOD(const int level, const int address)
{
	emit(PL0::LOD, level, address);
}

inline void emit_STO(const int level, const int address)
{
	emit(PL0::STO, level, address);
}

inline void emit_CAL(const int level, const int address)
{
	emit(PL0::CAL, level, address);
}

inline void emit_INT(const int value)
{
	emit(PL0::INT, 0, value);
}

inline void emit_JMP(const int address)
{
	emit(PL0::JMP, 0, address);
}

inline void emit_JMC(const int address)
{
	emit(PL0::JMC, 0, address);
}

inline void emit_RET()
{
	emit(PL0::RET, 0, 0);
}

inline void emit_REA()
{
	emit(PL0::REA, 0, 0);
}

inline void emit_WRI()
{
	emit(PL0::WRI, 0, 0);
}

inline void emit_NEW()
{
	emit(PL0::NEW, 0, 0);
}

inline void emit_DEL()
{
	emit(PL0::DEL, 0, 0);
}

inline void emit_LDA()
{
	emit(PL0::LDA, 0, 0);
}

inline void emit_STA()
{
	emit(PL0::STA, 0, 0);
}

inline void emit_PLD()
{
	emit(PL0::PLD, 0, 0);
}

inline void emit_PST()
{
	emit(PL0::PST, 0, 0);
}

#endif // __PL0_H_