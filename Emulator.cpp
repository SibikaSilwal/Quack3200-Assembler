#include "stdafx.h"
#include "Emulator.h"
/*
NAME

	InsertMemory - Inserts the machine code in the correct location in Quack3200 memory.

SYNOPSIS

	bool InsertMemory(int a_location, int a_contents);

DESCRIPTION

	This function will insert machine code "a_content" into its location "a_location"
	in the Quack3200 memory.

RETURNS

	The function returns true if the memory has not been exceeded otherwise returns false.
*/
bool emulator::InsertMemory(int a_location, int a_contents)
{
	//checks more memory and inserts translated machine code into memory location
	if (a_location >= 0 && a_location < 100000)
	{
		m_memory[a_location] = a_contents;
		return true;
	}
	else
	{
		cout << "Error, memory overflow" << endl;
		return false;
	}
}
/*
NAME

	RunProgram - runs the program in emulator.

SYNOPSIS

	bool RunProgram();

DESCRIPTION

	This function will analyze the machine code and will run the program. This function
	performs all the calculations in the program according to the opcode values in the 
	machine code.

RETURNS

	The function returns true if the machine code has the correct translation for opcode,
	register value, and memory address and therefore, the program is runable. Otherwise returns
	false.
*/
bool emulator::RunProgram()
{
	int loc = 100;
	int opcode;
	int content;
	string regg, str;
	int reg_value;
	int address;
	while (loc<MEMSZ) 
	{
		//content is an integer that stores the machinecode at the memory location, "loc"
		content = m_memory[loc];
		
		//converting machine code from integer to string. Now str has string machine code and content has integer machine code
		str = to_string(content);
		
		//extracting opcode from the machine code
		opcode = content / 1000000;
		
		//extracting register value from string machine code (str)
		if (str.size() == 7) regg = str[1];
		if (str.size() == 8) regg = str[2];
		
		//converting register value from string to integer
		reg_value = stoi(regg);
		
		//extracting the memory address from machine code
		address = content % 100000;
		
		switch (opcode)
		{
		//opcode 1 i.e. add, adds the values in register and memory and stores back to register.
		case 1:
			reg[reg_value] += m_memory[address];
			loc += 1;
			break;
		//opcode 2 i.e. sub, substracts the value in memory from register and stores back to register.  
		case 2:
			reg[reg_value] -= m_memory[address];
			loc += 1;
			break;
		//opcode 3 i.e. mult, multiplies the values in register and memory and stores back to register.
		case 3:
			reg[reg_value] *= m_memory[address];
			loc += 1;
			break;
		//opcode 4 i.e. div, divides the values in register by memory and stores back to register.
		case 4:
			reg[reg_value] /= m_memory[address];
			loc += 1;
			break;
		//opcode 5 i.e. load, stores the content of memory to register.
		case 5:
			reg[reg_value] = m_memory[address];
			loc += 1;
			break;
		//opcode 6 i.e. store, stores the content of register to memory.
		case 6:
			m_memory[address] = reg[reg_value];
			loc += 1;
			break;
		//opcode 7 i.e. read, reads a line from user and stores the number found in the memory.
		case 7:
			int userinput;
			cout << "?" << endl;
			cin >> userinput;
			m_memory[address] = userinput;
			loc += 1;
			break;
		//opcode 8 i.e. write, displays the content of memory to the console output.
		case 8:
			cout << m_memory[address] << endl;
			loc += 1;
			break;
		//opcode 9 i.e. branch, jumps to the specified address for next instruction.
		case 9:
			loc = address;
			break;
		//opcode 10 i.e. branch minus, jumps to the specified address for next instruction if the register value < 0.
		case 10:
			if (reg[reg_value] < 0)
			{
				loc = address;
			}
			else
			{
				loc += 1;
			}
			break;
		//opcode 11 i.e. branch zero, jumps to the specified address for next instruction if the register value equals 0.
		case 11:
			if (reg[reg_value] == 0)
			{
				loc = address;
			}
			else
			{
				loc += 1;
			}
			break;
		//opcode 12 i.e. branch positive, jumps to the specified address for next instruction if the register value > 0.
		case 12:
			if (reg[reg_value] > 0) 
			{
				loc = address;
			}
			else
			{
				loc += 1;
			}
			break;
		//opcode 13 i.e. halt, terminates execution. 
		case 13:
			exit(1);
			break;
		default:
			cout << "error!!" << endl;
			return false;
		}
		
	}
	return true;
}