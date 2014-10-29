#include "GGError.h"
using namespace std;

GGError::GGError( const string _fileName, const unsigned _fileLine )
{
	try
	{
		m_errorMessage = "File name: ";
		m_errorMessage += _fileName;
		m_errorMessage += " in line: ";
		m_errorMessage += to_string( _fileLine );
	}
	catch( ... )
	{

	}
}

const char * GGError::what() const noexcept
{
	return m_errorMessage.c_str();
}
