#pragma once

/// <summary> Клас за изчитане на пакети от конзолата </summary>
class CProtocolPackageConsoleReader
{
///////////////////////////
/// Constructors/Destructor
private:
	CProtocolPackageConsoleReader();
	~CProtocolPackageConsoleReader();

///////////////////////////
/// Methods
public:
	/// <summary> Изчита ред от входните данни </summary>
	/// <param name="pLine"> Пойнтер към масива с данни изчетен от конзолата </param>
	/// <param name="nLineBytesCount"> Размер на масива </param>
	/// <return> true при успех, false при грешка </return>
	static bool ReadInputLine( char*& const pLine, size_t& nLineBytesCount );
};

