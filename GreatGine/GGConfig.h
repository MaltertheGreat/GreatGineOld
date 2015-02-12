#pragma once

#include <map>

class GGConfig
{
public:
	GGConfig();

public:
	int GetInt( std::string _configName ) const;
	float GetFloat( std::string _configName ) const;

private:
	std::map< std::string, int > m_ints;
	std::map< std::string, float > m_floats;
};