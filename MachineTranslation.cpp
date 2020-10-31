#include "stdafx.h"
#include "MachineTranslation.h"
#include "Errors.h"
/*
NAME

	GetTranslationMachinecode - gets the machine translation for a machine instruction.

SYNOPSIS

	bool GetTranslationMachinecode(int &a_machinecode, Instruction a_inst, SymbolTable a_symtab);

DESCRIPTION

	This function will generate the machine translation "a_machinecode" for the machine 
	instruction a_inst.

RETURNS
	
	This function returns true if the labels/ operands in the instruction are present 
	in symboltable and the machine code can be generated, otherwise returns false.
*/
bool MachineTranslation::GetTranslationMachinecode(int &a_machinecode, Instruction a_inst, SymbolTable a_symtab)
{
	int symbol_loc;
	if (a_symtab.LookupSymbol(a_inst.GetOperand(), symbol_loc))
	{
		a_machinecode = (a_inst.GetNumericOpCode(a_inst.GetOpcode()) * 1000000)
			+ (a_inst.GetNumericRegister() * 100000) + symbol_loc;
		return true;
	}
	else if (a_inst.GetNumericOpCode(a_inst.GetOpcode()) == 13 && a_inst.GetOperand().empty())
	{
		a_machinecode = (13 * 1000000) + (9 * 100000) + 0; //using register 9, if no reg value
		return true;
	}
	else
	{
		return false;
	}
}
/*
NAME

	GetTranslationAssemblycode - gets the machine translation for an assembler instruction.

SYNOPSIS

	bool GetTranslationAssemblycode(int &a_machinecode, Instruction a_inst, SymbolTable a_symtab);

DESCRIPTION

	This function will generate the machine translation "a_machinecode" for the assembler instruction
	a_inst.

RETURNS

	This function returns true if the labels/ operands in the instruction are present
	in symboltable and the machine code can be generated, otherwise returns false.
*/
bool MachineTranslation::GetTranslationAssemblycode(int &a_machinecode, Instruction a_inst, SymbolTable a_symtab)
{
	int symbol_loc;
	if (a_symtab.LookupSymbol(a_inst.GetLabel(), symbol_loc))
	{
		if (a_inst.IsNumericOperand())
		{
			if (a_inst.GetOperandValue() <= 99999)
			{
				a_machinecode = (0 * 1000000) + (0 * 100000) + a_inst.GetOperandValue();
			}
			else
			{
				Errors::RecordError(to_string(a_inst.GetOperandValue()) + " is an invalid Operand size.");
			}
		}
		else //incase of ds
		{
			a_machinecode = 0;
		}
		return true;
	}
	else if (!a_inst.isLabel()) //it can be org statement
	{
		a_machinecode = 0;
		return true;
	}
	return false;
}