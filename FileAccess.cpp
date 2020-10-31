
//  Implementation of file access class.

#include "stdafx.h"
#include "FileAccess.h"


/*
NAME

	FileAccess - constructor of fileaccess class.

SYNOPSIS

	FileAccess::FileAccess( int argc, char *argv[] );

DESCRIPTION

	It is a constructor of fileaccess class that checks if there is exactly one
	runtime parameter and if yes opens the file. If there is more than one or no
	runtime parameter the program terminates. It also terminated the program if
	the file could not be open.
*/

FileAccess::FileAccess( int argc, char *argv[] )
{
    // Check that there is exactly one run time parameter.
    if( argc != 2 ) {
        cerr << "Usage: Assem <FileName>" << endl;
        exit( 1 );
    }
    // Open the file. 
    m_sfile.open( argv[1], ios::in );

    // If the open failed, report the error and terminate.
    if( ! m_sfile ) {
        cerr << "Source file could not be opened, assembler terminated."
            << endl;
        exit( 1 ); 
    }
}

/*
NAME

	FileAccess - destructor of fileaccess class.

SYNOPSIS

	FileAccess::~FileAccess();

DESCRIPTION

	It is the destructor of fileaccess class that closes the file.
*/
FileAccess::~FileAccess( )
{
    m_sfile.close( );
}

/*
NAME

	GetNextLine - gets the next line from the file.

SYNOPSIS

	bool GetNextLine( string &a_buff );

DESCRIPTION

	This function will get the next line from the file if there is any.

RETURNS

	The function returns true if there is more data to be passed otherwise returns false 
	if no more data is left in the file to read.
*/

bool FileAccess::GetNextLine( string &a_buff )
{
    // If there is no more data, return false.
    if( m_sfile.eof() ) {
    
        return false;
    }
    getline( m_sfile, a_buff );
    
    // Return indicating success.
    return true;
}

/*
NAME

	Rewind - goes back to the beginning of the file.

SYNOPSIS

	bool Rewind();

DESCRIPTION

	This function will clean the file flag and will go back to the beginning of the file.
*/

void FileAccess::Rewind( )
{
    m_sfile.clear();
    m_sfile.seekg( 0, ios::beg );
}
    
