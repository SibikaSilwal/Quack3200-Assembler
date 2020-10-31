/*
NAME

	Emulator - Emulator class supports the emulation of Quack3200 programs.

DESCRIPTION

	This class inserts the machine code into correct locations of Quack3200 memory. It gives out 
	actual output of the code by analyzing the description of each opcode.
*/

#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:

    const static int MEMSZ = 100000;	// The size of the memory of the Quack3200.
    emulator() {

        memset( m_memory, 0, MEMSZ * sizeof(int) );
    }
    // Records instructions and data into Quack3200 memory.
    bool InsertMemory( int a_location, int a_contents );
    
    // Runs the Quack3200 program recorded in memory.
    bool RunProgram( );

private:

    int m_memory[MEMSZ];       // The memory of the Quack3200.
    int reg[10];			  // The accumulator for the Quack3200
};

#endif

