#include <iostream>
#include "ProtocolPackageParser.h"

/// <summary> Хедар на пакета <sumamry>
#define PROTOCOL_PACKAGE_HEADER				"44 43"
#define PROTOCOL_PACKAGE_HEADER_SUM         ( (int)0x43 + (int)0x44 )
/// <summary> Символ за терминираща нула <sumamry>
#define TERMINATED_ZERO_SYMBOL              '\0'

///////////////////////////
/// Constructors/Destructor

CProtocolPackageParser::CProtocolPackageParser()
    : m_nSumOfAllhexDecimalsIntThePackage( 0 )
{
}

CProtocolPackageParser::~CProtocolPackageParser()
{
}

void CProtocolPackageParser::ProtocolParse( const char* pData, const size_t nDataBytesCount, Protocol* pResult )
{
    // Ако инстанцията на обекта се извика втори път, трябва да занулим и тук мембъра
    m_nSumOfAllhexDecimalsIntThePackage = 0;

    if ( !pData || nDataBytesCount <= 0 )
    {
        pResult->result = Protocol::PARSE_MESSAGE_EMPTY;
        return;
    }
        
    // Вземаме хедъра на пакета
    const char* pFirstByteOfTheHeader = strstr( pData, PROTOCOL_PACKAGE_HEADER );
        
    if ( !pFirstByteOfTheHeader )
    {
        pResult->result = Protocol::PARSE_WRONG_HEADER;
        return;
    }

    // Отиваме на първия значещ байт след хедъра
    const char* pCurrentChar = pFirstByteOfTheHeader + sizeof( PROTOCOL_PACKAGE_HEADER );

    if ( !ParseSizeAndIdentifier( pCurrentChar, pResult ) )
        return;

    if ( !ParseData( pCurrentChar, pResult ) )
        return;

    bool bIsEndOfTheArray = false;
    if ( !GetToNextChar( pCurrentChar, bIsEndOfTheArray ) || bIsEndOfTheArray )
    {
        pResult->result = Protocol::PARSE_WRONG_CHECKSUM;
        return;
    }

    if ( !ParseCheckSum( pCurrentChar, pResult ) )
        return;
}

bool CProtocolPackageParser::GetToNextChar( const char*& pCurrentChar, bool& bIsEndOfTheArray, const bool bSkipSpaces /* = true */ )
{
    bIsEndOfTheArray = false;

    if ( !pCurrentChar )
        return false;
    
    bIsEndOfTheArray = IsSymbolTerminatedZero( *pCurrentChar );
    if ( bIsEndOfTheArray )
        return true;

    ++pCurrentChar;

    bIsEndOfTheArray = IsSymbolTerminatedZero( *pCurrentChar );
    if ( bIsEndOfTheArray )
        return true;

    if ( std::isspace( *pCurrentChar ) && bSkipSpaces )
    {
        do
        {
            ++pCurrentChar;
            bIsEndOfTheArray = IsSymbolTerminatedZero( *pCurrentChar );
        } while ( std::isspace( *pCurrentChar ) || bIsEndOfTheArray );
    }

    return true;
}

bool CProtocolPackageParser::ParseSizeAndIdentifier( const char*& pCurrentChar, Protocol* pResult )
{
    uint8_t nCombinedHex = 0;
    if ( !CombineNextTwoCharsAsHex( pCurrentChar, nCombinedHex ) )
    {
        pResult->result = Protocol::PARSE_MESSAGE_EMPTY;
        return false;
    }

    // Вземаме първите 4 бита за брой байтове със същински данни
    // mask 0XF = 00001111
    pResult->numDataBytes = ( ( nCombinedHex >> 4 ) & 0xF );

    // Вземаме следващите 4 бита за идентификатор на пакета
    pResult->identifier = ( nCombinedHex & 0xF );

    if ( pResult->numDataBytes <= 0 )
    {
        pResult->result = Protocol::PARSE_MESSAGE_EMPTY;
        return false;
    }

    return true;
}

bool CProtocolPackageParser::ParseData( const char*& pCurrentChar, Protocol* pResult )
{
    uint8_t nCombinedHex = 0;
    bool bIsEndOfTheArray = false;
    bool bHasAtLeastOneDataByte = false;

    for ( int nDataIndex = 0; nDataIndex < pResult->numDataBytes; ++nDataIndex )
    {
        if ( !GetToNextChar( pCurrentChar, bIsEndOfTheArray ) || bIsEndOfTheArray )
        {
            pResult->result = Protocol::PARSE_NOT_ENOUGH_DATA;
            return false;
        }

        if( !CombineNextTwoCharsAsHex( pCurrentChar, nCombinedHex ) )
        {
            pResult->result = Protocol::PARSE_NOT_ENOUGH_DATA;
            return false;
        }

        // Няма нужда да проверяваме дали ще станат повече от 15
        // тъй като броят се пази в 4 бита, тоест максимумът е 15
        pResult->dataBytes[nDataIndex] = nCombinedHex;

        if ( !bHasAtLeastOneDataByte )
            bHasAtLeastOneDataByte = true;
    }     

    if ( !bHasAtLeastOneDataByte )
    {
        pResult->result = Protocol::PARSE_NOT_ENOUGH_DATA;
        return false;
    }

    return true;
}

bool CProtocolPackageParser::ParseCheckSum( const char*& pCurrentChar, Protocol* pResult )
{
    // Вземаме си hex-a на checksum-aта, но без да добавяме към общия сбор на останалите
    uint8_t nPackageCheckSum = 0;
    if( !CombineNextTwoCharsAsHex( pCurrentChar, nPackageCheckSum, false ) )
    {
        pResult->result = Protocol::PARSE_WRONG_CHECKSUM;
        return false;
    }

    // В мембъра на класа пазим всеки от hex-овете, но трябва да прибавим и фискирания хедър,
    // за да изчислим очакваната чек сума
    const int nCalculatedCheckSum = ( m_nSumOfAllhexDecimalsIntThePackage + PROTOCOL_PACKAGE_HEADER_SUM ) % 256;

    if ( (int)nPackageCheckSum != nCalculatedCheckSum )
    {
        pResult->result = Protocol::PARSE_WRONG_CHECKSUM;
        return false;
    }

    return true;
}

bool CProtocolPackageParser::CombineNextTwoCharsAsHex( const char*& pCurrentChar, uint8_t& nCombinedHex, const bool bAddToSum /*= true*/ )
{
    nCombinedHex = 0;

    if ( !pCurrentChar || IsSymbolTerminatedZero( *pCurrentChar ) )
        return false;

    bool bIsEndOfTheArray = false;

    if ( std::isspace( *pCurrentChar ) && !GetToNextChar( pCurrentChar, bIsEndOfTheArray ) )
        return false;      

    if ( bIsEndOfTheArray || !pCurrentChar )
        return false;

    const unsigned short sFirstHex = GetHexFromChar( *pCurrentChar );

    if ( !GetToNextChar( pCurrentChar, bIsEndOfTheArray ) )
        return false;

    if ( bIsEndOfTheArray || !pCurrentChar )
        return false;

    const unsigned short sSecondHex = GetHexFromChar( *pCurrentChar );

    nCombinedHex = ( ( sFirstHex << 4 ) | sSecondHex );

    if ( bAddToSum )
        m_nSumOfAllhexDecimalsIntThePackage += (int)nCombinedHex;

    return true;
}

unsigned short CProtocolPackageParser::GetHexFromChar( const char& cChar )
{
    if ( isdigit( cChar ) )
        return ( cChar - '0' );
    else
        return ( ( tolower( cChar ) - 'a' ) + 10 );
}

bool CProtocolPackageParser::IsSymbolTerminatedZero( const char& cChar )
{
    return cChar == TERMINATED_ZERO_SYMBOL;
}