#ifndef __PL0_H_
#define __PL0_H_

#include <stack>

#include "types.h"
#include "common.h"

/*
Basic ISA for PL/0

LIT 0,A    		ulož konstantu A do zásobníku
OPR 0,A    		proveď instrukci A z tabulky níže (vysledek na vrcholu zásobníku)
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
LOD L,A    		ulož hodnotu proměnné z adr. L,A na vrchol zásobníku
STO L,A    		zapiš do proměnné z adr. L,A hodnotu z vrcholu zásobníku
CAL L,A    		volej proceduru A z úrovně L
INT 0,A   		zvyš obsah top-registru zásobníku o hodnotu A
JMP 0,A    		proveď skok na adresu A
JMC 0,A    		proveď skok na adresu A, je-li hodnota na vrcholu zásobníku 0
RET 0,0    		návrat z procedury (return)

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

typedef unsigned char instruction_t;
typedef unsigned int operation_t;

// Description of the extended PL/0 isa (see help tab): https://home.zcu.cz/~lipka/fjp/pl0/ 
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

typedef char instruction_symbol_t[Max_Instruction_Symbol_Size + 1];

constexpr instruction_symbol_t Instruction_Symbol_Table[Number_Of_Instructions] = 
{"LIT", "OPR", "LOD", "STO", "CAL", "RET", "INT", "JMP", "JMC", "REA", "WRI", "OPF", "RTI", "ITR", "NEW", "DEL", "LDA", "STA", "PLD", "PST"};


// CODE STORAGE

union TCode_Entry_Value
{
	int i;
	float f;
};

struct TCode_Entry
{
	instruction_t instruction;
	int param_1;				// is always integer
	TCode_Entry_Value param_2;
	bool is_float;
};

constexpr unsigned int Max_Code_Length = 10000;
typedef TCode_Entry code_t[Max_Code_Length];

// Global compiled code storage
extern code_t sCode;

extern int sCurrent_Level;

extern int sCurrent_Branch_Level;

extern unsigned int sCode_Length;

extern unsigned int sCurrent_Block_Address;


// IDENTIFIER STORAGE

constexpr unsigned int Max_Identifier_Table_Length = 500;

typedef TIdentifier identifier_table_t[Max_Identifier_Table_Length];

extern identifier_table_t sIdentifier_Table;
extern unsigned int sIdentifier_Count;

#endif // __PL0_H_