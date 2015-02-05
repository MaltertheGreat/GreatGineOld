#include "GGConfig.h"
#include "GGError.h"
#include <fstream>
#include <string>
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

		char configType;
		configLineStream.get( configType );

		string configName;
		getline( configLineStream, configName, '=' );

		string configValue;
		getline( configLineStream, configValue );

		switch( configType )
		{
		case 'C':	// character
			m_chars[ configName ] = configValue[ 0 ];
			break;
		case 'I':	// integer
			m_ints[ configName ] = stoi( configValue );
			break;
		default:
			continue;
		}
	}
}

char GGConfig::GetChar( string _configName ) const
{
	return m_chars.at( _configName );
}

int GGConfig::GetInt( string _configName ) const
{
	return m_ints.at( _configName );
}