#pragma once
#include <cstdint>
#include "Protocol.h"

/// <summary> Клас за разчитане на пакет от протокола </summary>
class CProtocolPackageParser
{
///////////////////////////
/// Constructors/Destructor
public:
	CProtocolPackageParser();
	~CProtocolPackageParser();

///////////////////////////
/// Methods
public:
	/// <summary> Парсва данните от пакета </summary>
	/// <param name="pData"> Масив с данните от пакета в суров вид </param>
	/// <param name="nDataBytesCount"> Размер на масива с данни </param>
	/// <param name="pResult"> Резултат от парсването </param>
	void ProtocolParse( const char* pData, const size_t nDataBytesCount, Protocol* pResult );

private:
	/// <summary> Мърдаме един символ напред в масива </summary>
	/// <param name="pCurrentChar"> Пойнтер към символа, към който сме в момента </param>
	/// <param name="bIsEndOfTheArray"> Дали сме достигнали края на масива </param>
	/// <param name="bSkipSpaces"> Дали да мръднем още веднъж напред, ако следващия символ е спейс </param>
	/// <return> true при успех, false при грешка </return>
	bool GetToNextChar( const char*& pCurrentChar, bool& bIsEndOfTheArray, const bool bSkipSpaces = true );

	/// <summary> Взема размера на значещите данни и иденитфикатора </summary>
	/// <param name="pCurrentChar"> Пойнтер към символа, към който сме в момента </param>
	/// <param name="pResult"> Резултат от парсването </param>
	/// <return> true при успех, false при грешка </return>
	bool ParseSizeAndIdentifier( const char*& pCurrentChar, Protocol* pResult );

	/// <summary> Взема същинските данни </summary>
	/// <param name="pCurrentChar"> Пойнтер към символа, към който сме в момента </param>
	/// <param name="pResult"> Резултат от парсването </param>
	/// <return> true при успех, false при грешка </return>
	bool ParseData( const char*& pCurrentChar, Protocol* pResult );

	/// <summary> Взема чексумата </summary>
	/// <param name="pCurrentChar"> Пойнтер към символа, към който сме в момента </param>
	/// <param name="pResult"> Резултат от парсването </param>
	/// <return> true при успех, false при грешка </return>
	bool ParseCheckSum( const char*& pCurrentChar, Protocol* pResult );

	/// <summary> Комбинира следващите 2 символа от масива в един hexdecimal </summary>
	/// <param name="pCurrentChar"> Пойнтер към символа, от който да започне комбианцията </param>
	/// <param name="nCombinedHex"> Комбинирания hexdecimal </param>
	/// <param name="bAddToSum"> Дали да добавяме към общата сума за пакета </param>
	/// <return> true при успех, false при грешка </return>
	bool CombineNextTwoCharsAsHex( const char*& pCurrentChar, uint8_t& nCombinedHex, const bool bAddToSum = true );

	/// <summary> Преобразува символ в hexdecimal </summary>
	/// <param name="cChar"> Символ </param>
	unsigned short GetHexFromChar( const char& cChar );

	/// <summary> Дали символът е терминираща нула </summary>
	/// <param name="cChar"> Символ </param>
	inline bool IsSymbolTerminatedZero( const char& cChar );

///////////////////////////
/// Members
private:
	/// <summary> Сумата на всички hex-ове в пакета </summary>
	int		m_nSumOfAllhexDecimalsIntThePackage;
};