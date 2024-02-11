#include "ProtocolPackageConsoleReader.h"
#include "ProtocolPackageParser.h"

/// <summary> Грешка при изчитане на входните данни от кознолата </summary>
#define ERROR_READ_INPUT_FROM_CONSOLE   -1

int main()
{
    char* pData = nullptr;
    size_t nDataBytesCount = 0;

    if ( !CProtocolPackageConsoleReader::ReadInputLine( pData, nDataBytesCount ) )
        return ERROR_READ_INPUT_FROM_CONSOLE;

    Protocol* pProtocolResult = new Protocol();
    CProtocolPackageParser().ProtocolParse( pData, nDataBytesCount, pProtocolResult );

    delete[] pData;
    delete pProtocolResult;
}