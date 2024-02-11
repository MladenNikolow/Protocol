#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <corecrt_memory.h>

/// <summary> Структура съдържаща данните за един пакет, трансфериран чрез протокола </summary>
struct Protocol 
{
	/// <summary> Резултат от парсването на пакета </summary>
	enum ParseResult : int
	{
		/// <summary> Успешно парснат пакет </summary>
		PARSE_OK					= 0

		/// <summary> Непълен пакет </summary>
	,	PARSE_NOT_ENOUGH_DATA		= 1

		/// <summary> Пакет без данни </summary>
	,	PARSE_MESSAGE_EMPTY			= 2

		/// <summary> Пакет с грешен хедър </summary>
	,	PARSE_WRONG_HEADER			= 3

		/// <summary> Пакет с грешна чексума </summary>
	,	PARSE_WRONG_CHECKSUM		= 4


		/// <summary> Общ брой опции за резултат от парсването </summary>
	,	PARSE_RESULTS_COUNT
	};

	Protocol()
		: result( PARSE_OK )
		, identifier( 0 )
		, numDataBytes( 0 )
	{
		memset(dataBytes, 0, 15);
	}

	std::string ResultToString() const
	{
		switch( result )
		{
		case PARSE_OK:
			return "PARSE_OK";

		case PARSE_NOT_ENOUGH_DATA:
			return "PARSE_NOT_ENOUGH_DATA";

		case PARSE_MESSAGE_EMPTY:
			return "PARSE_MESSAGE_EMPTY";

		case PARSE_WRONG_HEADER:
			return "PARSE_WRONG_HEADER";

		case PARSE_WRONG_CHECKSUM:
			return "PARSE_WRONG_CHECKSUM";

		default:
			return "";
		}
	}

	std::string DataBytesToString() const
	{
		if ( result != PARSE_OK && result != PARSE_WRONG_CHECKSUM )
			return "";

		std::stringstream stringStream;

		for ( int nDataIndex = 0; nDataIndex < numDataBytes; ++nDataIndex )
		{
			stringStream << "0x" << std::hex << static_cast< int >( dataBytes[nDataIndex] );
			if ( nDataIndex + 1 != numDataBytes )
				stringStream << ", ";
		}

		return stringStream.str();
	}

	void DisplayData()
	{
		std::cout << "Parse result: " << ResultToString() << "\n"
				  << "Identifier: " << identifier << "\n"
				  << "Number of the data bytes: " << numDataBytes << "\n"
				  << "Data bytes: " << DataBytesToString();
	}

	/// <summary> Резултат от парсването </summary>
	ParseResult			result;

	/// <summary> Идентификатор на пакета </summary>
	int					identifier;

	/// <summary> Брой байтове </summary>
	int					numDataBytes;

	/// <summary> Същинските данни </summary>
	char				dataBytes[15];
};
