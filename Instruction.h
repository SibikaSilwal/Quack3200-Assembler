/*
NAME

	Instruction - Instruction class is to parse and provide information about instructions.

DESCRIPTION

	This class allows us to parse the instruction into labels, operands, and opcodes. This class also gives us
	the information about the instruction type (machine language, assembler, comment, or endline). This class also
	generated the location for next instruction in the program.
*/

#pragma once

// The elements of an instruction.
class Instruction {

public:

    Instruction( ) { };
    ~Instruction( ) { };

    // Codes to indicate the type of instruction we are processing.  The enum is defined inside the class to 
	//limit the scope of the enum, as we will be determining InstructionType only in Instruction class in ParseInstruction.
    enum InstructionType {
        ST_MachineLanguage, 	// A machine language instruction.
        ST_AssemblerInstr,  		// Assembler Language instruction.
        ST_Comment,          		// Comment or blank line
        ST_End                   		// end instruction.
    };
    // Parse the Instruction.
	InstructionType ParseInstruction(string& a_buff);

    // Compute the location of the next instruction.
	int LocationNextInstruction(int a_loc);

    // To access the label
	inline string &GetLabel() {return m_Label;};
    
	// To determine if a label is blank.
    inline bool isLabel( ) {return ! m_Label.empty();};

	//To check if the label matches the criteria to be a valid label.
	inline bool GoodLabel() 
	{
		if (m_Label.size() <= 10 && isalpha(m_Label[0])) return true;
		return false;
	};

	//returns the symbolic operand for the instruction
	inline string &GetOperand() { return m_Operand; };

	//returns the numeric operand for the instruction
	inline int &GetOperandValue() { return m_OperandValue; };

	//returns the opcode for the instruction
	inline string &GetOpcode() { return m_OpCode; };

	//returns the numeric register value for the instruction
	inline int &GetNumericRegister() 
	{ 
		if (m_Register.empty()) m_NumRegister = 9;
		else if (IsNumber(m_Register))  m_NumRegister = stoi(m_Register);		
		else m_NumRegister = -1;
		return m_NumRegister;		 
	};

	//returns true if the operand is numeric.
	inline bool &IsNumericOperand() { return m_IsNumericOperand; };

	//converts a string to lower case, this function is used with opcodes only.
	inline string ToLower(string a_str)
	{ 
		transform(a_str.begin(), a_str.end(), a_str.begin(), ::tolower); 
		return a_str;
	};
	
	//returns the numberic operation code from string opcode
	int GetNumericOpCode(string &a_Opcode);

	//checks if a string is a number
	bool IsNumber(string a_string);

	//checks is an opcode is legal or not.
	bool ValidOpCode(string &a_opcode);
private:


    // The elemements of a instruction
    string m_Label;            // The label.
    string m_Register;	    // The register specified.  Use -1 if there is none.
    string m_OpCode;       // The symbolic op code.
    string m_Operand;      // The operand.
    string m_instruction;    // The original instruction.
	vector<string> m_SplitInst;
    // Derived values.
    int m_NumOpCode;     // The numerical value of the op code.
    int m_NumRegister;	   // the numberic value for the register.
    InstructionType m_type; // The type of instruction.

	bool m_IsNumericOperand;// == true if the operand is numeric.
    int m_OperandValue;   // The value of the operand if it is numeric.
	
	//array of opcodes
	string m_opcode[13] = { "add", "sub", "mult", "div", "load", "store", "read", "write", "b", "bm", "bz", "bp", "halt" };
};


