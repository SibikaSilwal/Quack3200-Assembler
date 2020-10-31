/*
NAME

	MachineTranslation - MachineTranslation class translates the instruction into machine code.

DESCRIPTION

	This class generates machine translation code for machine instruction and assembly instruction. It uses
	location, opcode values, and operand values to generate the machine code.
*/
#pragma once
#include "Instruction.h"
#include "SymTab.h"

class MachineTranslation {

public:
	
	MachineTranslation() {};
	~MachineTranslation() {};

	bool GetTranslationMachinecode(int &machinecode, Instruction m_inst, SymbolTable m_symtab);

	bool GetTranslationAssemblycode(int &machinecode, Instruction m_inst, SymbolTable m_symtab);

private:
};