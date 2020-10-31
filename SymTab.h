/*
NAME

	SymbolTable - SymbolTable class holds the actual symbol table which is a map. 

DESCRIPTION

	This class allows us to store the labels and their locations in the map and thus builds the symbol table.
	It also provides us with a function to look up symbols in the symbol table.
*/
#pragma once

class SymbolTable {

public:

   const int multiplyDefinedSymbol = -999;

    // Add a new symbol to the symbol table.
    void AddSymbol( string &a_symbol, int a_loc );

    // Display the symbol table.
	void DisplaySymbolTable();

    // Lookup a symbol in the symbol table.
	bool LookupSymbol(string& a_symbol, int& a_loc);// { return true; }

private:

    // This is the actual symbol table.  The symbol is the key to the map.
    map<string, int> m_symbolTable;

};
