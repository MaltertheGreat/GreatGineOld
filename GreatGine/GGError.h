#pragma once

#include <exception>
#include <string>

#define GG_THROW	throw GGError( __FILE__, __LINE__ )

class GGError : public std::exception
{
public:
	GGError( const std::string _fileName, const unsigned _fileLine );

	virtual const char* what() const noexcept;

private:
	std::string m_errorMessage;
};