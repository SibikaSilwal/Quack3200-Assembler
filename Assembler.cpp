//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"

// Constructor for the assembler.  Note: we are passing argc and argv to the file access constructor.
// See main program.  
Assembler::Assembler( int argc, char *argv[] )
: m_facc( argc, argv )
{}

//Destructor for the assembler.
Assembler::~Assembler( ){}

/*
NAME

	PassI - establishes the location of the labels.

SYNOPSIS

	void PassI();

DESCRIPTION

	This function will record the label, if the instruction has one,
	and its location in the symbol table.
*/
void Assembler::PassI( ) 
{
    int loc = 0;        // Tracks the location of the instructions to be generated.
    // Successively process each line of source code.
    for( ; ; ) {

        // Read the next line from the source file.
        string line; 
        if( ! m_facc.GetNextLine( line ) ) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st =  m_inst.ParseInstruction( line );

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement.
        if( st == Instruction::ST_End ) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip other instruction types.
        if( st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr ) 
        {
        	continue;
	    }
        // If the instruction has a label, record it and its location in the
        // symbol table.
        if( m_inst.isLabel( ) ) 
		{
            m_symtab.AddSymbol( m_inst.GetLabel( ), loc );
        }
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction( loc );
    }
}
/*
NAME

	PassII - checks for errors in the program code, records, and reports the error.

SYNOPSIS

	void PassII();

DESCRIPTION

	This function will check for the errors in the user program. If an error is found
	it records the error and displays the error after the offending statement. It also
	displays the machine translation of the instructions.
*/
void Assembler::PassII()
{
	int loc = 0; //tracks the location of the next instruction
	int mem_content=0; //stores the machine code
	m_facc.Rewind();
	cout << "        Translation of Program" << endl;
	cout << "Location" << "  " << "Content" <<"      "<< "Original Statement" << endl;
	while (true)
	{
		Errors::InitErrorReporting();
		string line;
		//if we reached the end of our file and yet have no end statement, reports error for missing "end".
		if (!m_facc.GetNextLine(line))
		{
			Errors::RecordError("No end statement.");
			return;
		}
		Instruction::InstructionType st = m_inst.ParseInstruction(line);

		//reports error if "end" is not the last statement.
		if (st == Instruction::ST_End)
		{
			while (m_facc.GetNextLine(line))
			{
				if (!line.empty())
				{
					Errors::RecordError("End is not the last statement.");
				}				
			}
			return;
		}

		//if not machine or assembler language no need to generate machine code. So skip to other isntructions.
		if (st != Instruction::ST_MachineLanguage && st != Instruction::ST_AssemblerInstr)
		{
			cout << setfill(' ')<<setw(30)<<" " << line << endl;
			continue;
		}
		//label can be present on machine or assembler instruction. So check for labels to be valid.
		if (m_inst.isLabel()) 
		{
			if (!m_inst.GoodLabel())
			{
				Errors::RecordError(m_inst.GetLabel() + " does not satisfy the criteria to be a label. Error!!");
			}
		}
		
		if (st == Instruction::ST_MachineLanguage)
		{
			//Checking for opcode validity.
			if (!m_inst.ValidOpCode(m_inst.GetOpcode()))
			{
				Errors::RecordError(m_inst.GetOpcode() + " is an illegal Opcode.");
			}
			//if machine instruction, operand has to be symbolic, not numeric
			if (isdigit(m_inst.GetOperand()[0]))
			{
				Errors::RecordError(m_inst.GetOperand() + " Syntax error in the construction of operand.");
			}
			//checking for valid register value
			if (m_inst.GetNumericRegister() <0|| m_inst.GetNumericRegister()>9)
			{
				Errors::RecordError("Invalid register value");
			}
			//machine translation for machine language instruction
			if(!m_trans.GetTranslationMachinecode(mem_content, m_inst, m_symtab))
			{
				Errors::RecordError(m_inst.GetOperand() + " operand did not match from the labels in symbol table.");
			}
		}
		if (st == Instruction::ST_AssemblerInstr)
		{
			//if assembler instruction, operand has to be numeric
			if (!m_inst.IsNumericOperand())
			{
				Errors::RecordError(m_inst.GetOperand() + " Syntax error in the construction of operand.");
			}
			//machine translation for assembly instruction
			if(!m_trans.GetTranslationAssemblycode(mem_content, m_inst, m_symtab))
			{
				Errors::RecordError(m_inst.GetLabel() + " is not present in symbol table.");
			}
		}

		//printing out machine translation code with location and original instruction
		cout << "   " << loc << "    " << setw(8) << setfill('0');
		(mem_content != 0) ? cout << mem_content : cout << "        " ;
		cout << setw(10) << setfill(' ')<<" " << line << endl;
		
		//passing machine code into Quack3200 memory
		m_emul.InsertMemory(loc, mem_content);

		//getting location for next instruction
		loc = m_inst.LocationNextInstruction(loc);
		if (loc > 99999)
		{
			Errors::RecordError("Insufficient memory for translation");
		}
		//Prints the current error if any
		if (currenterror) Errors::DisplayErrors();
	}
}
/*
NAME

	RunProgramInEmulator - runs the program in emulator.

SYNOPSIS

	void RunProgramInEmulator();

DESCRIPTION

	This function will run the program in emulator if no errors were recorded in PassII.
	If errors were recorded in PassII, diplays the list of errors.
*/
void Assembler::RunProgramInEmulator()
{
	if (m_ErrorMsgs.empty())
	{
		m_emul.RunProgram();
	}
	else
	{
		//switching current error to false so that the last error does not get printed twice.
		currenterror = false; 
		//to display all errors
		displayallerrors = true;
		cout << endl;
		cout << setfill('.')<<setw(100)<<" " << endl; //formatting
		cout << endl;
		cout << "Your program has following errors." << endl;
		Errors::DisplayErrors();
	}
	
}

