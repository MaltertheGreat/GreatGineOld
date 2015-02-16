#include "PCH.h"
#include "GGConfig.h"
#include "GGError.h"
#include <fstream>
#include <sstream>
using namespace std;

GGConfig::GGConfig()
{
	ifstream configFile( "config.txt" );
	string configLine;

	if( configFile.bad() )
	{
		GG_THROW;
	}

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
		case 'H':	// hex integer
			m_ints[ configName ] = stoi( configValue, 0, 16 );
			break;
		case 'F':	// float
			m_floats[ configName ] = stof( configValue );
			break;
		default:
			continue;
		}
	}
}

int GGConfig::GetInt( string _configName ) const
{
	return m_ints.at( _configName );
}

float GGConfig::GetFloat( std::string _configName ) const
{
	return m_floats.at( _configName );
}