#include "ProtocolPackageConsoleReader.h"
#include <iostream>
#include <string>

CProtocolPackageConsoleReader::CProtocolPackageConsoleReader()
{
}

CProtocolPackageConsoleReader::~CProtocolPackageConsoleReader()
{
}

/* static */
bool CProtocolPackageConsoleReader::ReadInputLine( char*& const pLine, size_t& nLineBytesCount )
{
    std::cout << "Please enter your package input: ";

    std::string strInputLine;
    if ( !std::getline( std::cin, strInputLine ) )
    {
        std::cout << "Error reading the input." << std::endl;
        return false;
    }

    if ( strInputLine.empty() )
        return true;

    nLineBytesCount = strInputLine.size() + 1;
    pLine = new char[ nLineBytesCount ];

    for ( size_t nSymbolIndex = 0; nSymbolIndex < strInputLine.size(); ++nSymbolIndex )
        pLine[ nSymbolIndex ] = strInputLine.at( nSymbolIndex );

    pLine[ strInputLine.size() ] = '\0';

	return true;
}