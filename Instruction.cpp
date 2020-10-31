#include "stdafx.h"
#include "Instruction.h"
#include "FileAccess.h"
#include "SymTab.h"
#include "Errors.h"
/*
NAME

	ParseInstruction - Parses one instruction at a time into comment, label, operand, 
   						register value, and opcode.

SYNOPSIS

	Instruction::InstructionType ParseInstruction(string& a_buff);

DESCRIPTION

	This function will parse the string "a_buff" and obtain the label, opcode, register value,
	operand, and comment. This function identifies if there is no label or register value. If 
	no register value found in the instruction, it sets the register value as 9. 

RETURNS
	This function returns the instruction type (endline, comment, machine instruction, or 
	assembler instruction).
*/
Instruction::InstructionType Instruction::ParseInstruction(string& a_buff)
{
	//makes sure the vector and strings are empty before every parse.
	m_SplitInst.clear(); m_Label.clear(); m_OpCode.clear(); m_Operand.clear(); m_Register.clear();
	m_OperandValue = 0; //default operand value = 0
	m_IsNumericOperand = false;
	m_instruction = a_buff; 
	
	//replacing ',' with space and erasing comments.
	for (int i = 0; i < m_instruction.size(); i++)
	{
		if (m_instruction[i] == ',') m_instruction[i] = ' ';
		if (m_instruction[i] == ';') m_instruction.erase(i, (m_instruction.size()));
	}
	// creating iss object inorder to break string to words
	istringstream buff_word(m_instruction);
	do
	{
		string word;
		buff_word >> word;
		m_SplitInst.push_back(word); //m_SplintInst is a vector that holds each word in the instruction.
	} while (buff_word);
	m_SplitInst.pop_back();
	
	//if empty string return comment
	if (m_SplitInst.size() == 0) return ST_Comment;
	
	//if an instruction only has "end" return end statement
	if (m_SplitInst.size()==1 && (ToLower(m_SplitInst[0]) == "end"))return ST_End;

	//If dc, ds, or org is present in an instruction, parses it into label and operand and returns assembler instruction
	if (m_SplitInst.size() == 3 && (ToLower(m_SplitInst[1]) == "dc" || ToLower(m_SplitInst[1]) == "ds"))
	{
		m_Label = m_SplitInst[0];
		if (IsNumber(m_SplitInst[2]))
		{
			m_IsNumericOperand = true;
			if(ToLower(m_SplitInst[1]) == "dc" ) m_OperandValue = stoi(m_SplitInst[2]);
		}
		return ST_AssemblerInstr;
	}
	if (m_SplitInst.size() == 2 && ToLower(m_SplitInst[0]) == "org" && IsNumber(m_SplitInst[1]))
	{
		m_IsNumericOperand = true;
		return ST_AssemblerInstr;
	}

	//checks for machine instructions, by identifying the presence of opcodes.
	if (m_SplitInst.size() > 0 && m_SplitInst.size() < 3)
	{
		//size 2 machine instruction has a label only with opcode halt
		if (m_SplitInst.size()==2 && ToLower(m_SplitInst[1]) == m_opcode[12])
		{
			m_Label = m_SplitInst[0];
			m_OpCode = m_SplitInst[1];
		}
		else m_OpCode = m_SplitInst[0]; //otherwise no register, no label
	}
	
	//for size 3, if opcode is read, write, or branch then label exists
	if (m_SplitInst.size() == 3)
	{
		int i = 0; bool label_exists = true; //boolean variable to check whether label exists or not
		while (i < 13) 
		{
			if (ToLower(m_SplitInst[0]) == m_opcode[i]) //if the first string is opcode, there is no label
			{
				m_OpCode = m_SplitInst[0];
				m_Register = m_SplitInst[1];
				label_exists=false;
			}
			i++;
		}
		if (label_exists) //else there is label
		{
			m_Label = m_SplitInst[0];
			m_OpCode = m_SplitInst[1];
		}
	}
	//if a machine instruction has size = 3, it must have a label
	if (m_SplitInst.size()==4)
	{
		m_Label = m_SplitInst[0];
		m_OpCode = m_SplitInst[1];
		m_Register = m_SplitInst[2]; 
	}

	//if an instruction has size > 4, it is a syntax error.
	if (m_SplitInst.size() > 4) Errors::RecordError("Too long instruction. Syntax error.");

	//a machine instruction with size>1, always has an operand at the end except for halt instruction.
	if (m_SplitInst.size() > 1&&ToLower(m_SplitInst[1])!=m_opcode[12] ) m_Operand = m_SplitInst.back();
	
	return ST_MachineLanguage; 
}

/*
NAME

	LocationNextInstruction - Returns the location of next instruction in the program code.

SYNOPSIS

	int LocationNextInstruction(int a_loc);

DESCRIPTION

	This function will calculates the location "a_loc" for next instruction in the program.
	It identifies assembler opcode like define storage(ds)/ org and determines location accordingly.

RETURNS
	This function returns the location for next instruction.

*/
int Instruction::LocationNextInstruction(int a_loc) 
{   
	if (m_SplitInst.size() > 0) {
		//when there is the keyword "org" value next to it will be the location for next instruction
		if (ToLower(m_SplitInst[0]) == "org") 
		{
			a_loc = stoi(m_SplitInst[1]);
		}
		//when there is the keyword "ds" value next to it + initial location will be the location for next instruction
		else if ((m_SplitInst.size() > 1) && ToLower(m_SplitInst[1]) == "ds")
		{
			if(IsNumber(m_SplitInst[2])) a_loc = a_loc + stoi(m_SplitInst[2]);
			else Errors::RecordError("Location cannot be generated.");
		}
		else //otherwise location increases by 1 for every next instruction
		{
			a_loc = a_loc + 1;
		}
	}
	return a_loc;
}
/*
NAME

	ValidOpCode - Checks if the op_code is correct and valid.

SYNOPSIS

	bool ValidOpCode(string &a_opcode);

DESCRIPTION

	This function will check for the validity of the opcode "a_opcpde" by comparing it with the elements of opcode array.

RETURNS
	This function returns true if "a_opcode" is a valid opcode and false if not.
*/
bool Instruction::ValidOpCode(string &a_opcode)
{
		for (int i = 0; i < 13; i++) 
		{
			if (ToLower(a_opcode) == m_opcode[i]) return true;
		}
		return false;
}
/*
NAME

	GetNumericOpCode - Returns the numberic opcode value for the given opcode string.

SYNOPSIS

	int GetNumericOpCode(string &a_Opcode);

DESCRIPTION

	This function will generate the numberic opcode value for the string opcode "a_Opcode".

RETURNS

	This function returns the numberic opcode value for "a_Opcode".
*/
int Instruction::GetNumericOpCode(string &a_Opcode)
{
	for (int i = 0; i < 13; i++) 
	{
		if (ToLower(a_Opcode) == m_opcode[i]) return i + 1;
	}
	return 0;
}
/*
NAME

	IsNumber - Checks if a string is an integer.

SYNOPSIS

	bool IsNumber(string a_string);

DESCRIPTION

	This function will check if a string "a_string" is a number or not

RETURNS

	This function returns true if "a_string" is a number otherwise returns false.
*/
bool Instruction::IsNumber(string a_string)
{
	for (int i = 0; i < a_string.length(); i++)
	{
		if (!isdigit(a_string[i]))
			return false;
	}
	return true;
}