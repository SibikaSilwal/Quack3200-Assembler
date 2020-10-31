//
//		Implementation of the symbol table class. 
//
#include "stdafx.h"
#include "SymTab.h"
#include "Errors.h"

/*
NAME

    AddSymbol - adds a new symbol to the symbol table.

SYNOPSIS

    void AddSymbol( char *a_symbol, int a_loc );

DESCRIPTION

    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
void SymbolTable::AddSymbol( string &a_symbol, int a_loc )
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st;
    st = m_symbolTable.find( a_symbol );
    if( st != m_symbolTable.end() ) {

        st->second = multiplyDefinedSymbol;
		Errors::RecordError(a_symbol+ "is Multiply defined labels.");
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*
NAME

	DisplaySymbolTable - Displays the symbol table in console output.

SYNOPSIS

	void DisplaySymbolTable();

DESCRIPTION

	This function will display the symbol table i.e. it displays labels and
	their locations in Quack3200 memory.
*/
void SymbolTable::DisplaySymbolTable()
{
	map<string, int>::iterator itr;
	cout << "Symbol Table: "<<endl;
	cout << "Symbol#   Symbol     Location"<<endl;
	int i = 0;
	for (itr = m_symbolTable.begin(); itr != m_symbolTable.end(); ++itr) 
	{
		cout<<i<< right << setfill(' ') << setw(12) << itr->first 
			<< right << setfill(' ') << setw(12) << itr->second << endl;
		i++;
	}
	cout << endl;
	cout << setfill('*')<<setw(100)<<" "<< endl;
	cout << endl;
}
/*
NAME

	LookupSymbol - Looks up for the symbol passed into the function in the symbol table.

SYNOPSIS

	bool LookupSymbol(string& a_symbol, int& a_loc);

DESCRIPTION

	This function will look for the symbol "a_symbol" in the symbol table.

RETURNS
	If the symbol is present the function returns true and the location of the symbol 
	is assigned to "a_loc" else returns false.
*/
bool SymbolTable::LookupSymbol(string& a_symbol, int& a_loc)
{
	map<string, int>::iterator st;
	st = m_symbolTable.find(a_symbol);
	if (st != m_symbolTable.end())
	{
		a_loc = m_symbolTable.at(a_symbol);
		return true;
	}
	return false;
}
