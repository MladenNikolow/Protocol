#pragma once
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

	/// <summary> Резултат от парсването </summary>
	ParseResult			result;

	/// <summary> Идентификатор на пакета </summary>
	int					identifier;

	/// <summary> Брой байтове </summary>
	int					numDataBytes;

	/// <summary> Същинските данни </summary>
	char				dataBytes[15];
};