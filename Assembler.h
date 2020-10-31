/*
NAME

	Assembler - Assembler class is a container for all the components that make up the assembler.

DESCRIPTION

	This class puts everything together to finally run the assembler in main.cpp (Assem.cpp). It contains function
	that establishes locations of the symbols, displays symbol table and the machine translation, and  identifies
	and records all the errors in the code. It also allows our program to finally run in the emulator if no errors
	are recorded.
*/
#pragma once 

#include "SymTab.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "Emulator.h"
#include "MachineTranslation.h"


class Assembler {

public:
    Assembler( int argc, char *argv[] );
    ~Assembler( );

    // Pass I - establish the locations of the symbols
    void PassI( );

    // Pass II - generate a translation
    void PassII( );

    // Display the symbols in the symbol table.
    void DisplaySymbolTable() { m_symtab.DisplaySymbolTable(); }
    
    // Run emulator on the translation.
    void RunProgramInEmulator();

private:

    FileAccess m_facc;	    // File Access object
    SymbolTable m_symtab; // Symbol table object
    Instruction m_inst;	    // Instruction object
    emulator m_emul;        // Emulator object
	MachineTranslation m_trans; //Machine Translation object
};

