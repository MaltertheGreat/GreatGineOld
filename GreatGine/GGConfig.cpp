#include "PCH.h"
#include "GGConfig.h"
#include "GGError.h"
#include <fstream>
#include <sstream>
using namespace std;

GGConfig::GGConfig()
	:
	m_configFileName( "config.txt" )
{
	ifstream configFile( m_configFileName );
	if( !configFile.is_open() )
	{
		configFile.open( m_configFileName, ios::in | ios::out | ios::trunc );
	}
	if( !configFile.good() )
	{
		GG_THROW;
	}

	string configLine;
	while( getline( configFile, configLine ) )
	{
		if( configLine[ 0 ] == '#' )	// comment
		{
			continue;
		}

		istringstream configLineStream( configLine );

		char configType = 0;
		configLineStream.get( configType );

		string configName;
		getline( configLineStream, configName, '=' );

		string configValue;
		getline( configLineStream, configValue );

		switch( configType )
		{
			case 'I':	// integer
				m_ints[ configName ] = stoi( configValue );
				break;
			case 'U':	// unsigned
				m_uints[ configName ] = stoul( configValue );
				break;
			case 'F':	// float
				m_floats[ configName ] = stof( configValue );
				break;
			default:
				continue;
		}
	}
}

int GGConfig::GetInt( string _configName, int _defaultValue )
{
	if( m_ints.count( _configName ) == 0 )
	{
		ofstream configFile( m_configFileName, ios::app );
		string configLine = "I" + _configName + "=" + to_string( _defaultValue );
		configFile << configLine << endl;

		m_ints[ _configName ] = _defaultValue;
	}

	return m_ints.at( _configName );
}

UINT GGConfig::GetUint( std::string _configName, UINT _defaultValue )
{
	if( m_uints.count( _configName ) == 0 )
	{
		ofstream configFile( m_configFileName, ios::app );
		string configLine = "U" + _configName + "=" + to_string( _defaultValue );
		configFile << configLine << endl;

		m_uints[ _configName ] = _defaultValue;
	}

	return m_uints.at( _configName );
}

float GGConfig::GetFloat( std::string _configName, float _defaultValue )
{
	if( m_floats.count( _configName ) == 0 )
	{
		ofstream configFile( m_configFileName, ios::app );
		string configLine = "F" + _configName + "=" + to_string( _defaultValue );
		configFile << configLine << endl;

		m_floats[ _configName ] = _defaultValue;
	}

	return m_floats.at( _configName );
}