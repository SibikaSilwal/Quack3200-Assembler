/*
NAME

	Errors - Errors class manages errors reporting. All members in this class are static thus, can be accessed anywhere.

DESCRIPTION

	This class records error for each instruction being parsed in PASSII(assembler class) if any. Errors are displayed
	after every offending statement. Also the list of all errors are displayed after the machine tranlation is complete 
	by DisplayErrors function.
*/
#ifndef _ERRORS_H
#define _ERRORS_H
#include"stdafx.h"
#include "Assembler.h"

static vector<string> m_ErrorMsgs;
static bool currenterror = false;
static bool displayallerrors = false;
class Errors {

public:
    
    // Initializes error reports.
	static void InitErrorReporting() 
	{
		currenterror = false;
		displayallerrors = false;
	};

    // Records an error message if any for every instruction.
    static void RecordError(string a_emsg )
	{
		m_ErrorMsgs.push_back(a_emsg);
		currenterror = true;
	};

    // Displays the collected error message as well as current error after the offending statement.
	static void DisplayErrors()
	{
		//displays current error only, if any
		if (currenterror == true) cout << setfill(' ') << setw(15) << " " << "----"<< m_ErrorMsgs.back()<<"----" << endl;
		if (displayallerrors == true) //dsiplays all errros.
		{
			for (auto x : m_ErrorMsgs)
			{
				cout << x << endl;
			}
		}
	};
		

private:	
};
#endif