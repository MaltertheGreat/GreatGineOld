#pragma once

#include <fstream>
#include <map>

class GGConfig
{
public:
	GGConfig();

public:
	int GetInt( std::string _configName, int _defaultValue );
	UINT GetUint( std::string _configName, UINT _defaultValue );
	float GetFloat( std::string _configName, float _defaultValue );

private:
	std::string m_configFileName;

	std::map< std::string, int > m_ints;
	std::map< std::string, UINT > m_uints;
	std::map< std::string, float > m_floats;
};