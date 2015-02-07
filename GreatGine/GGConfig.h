#pragma once

#include <map>

class GGConfig
{
public:
	GGConfig();

public:
	int GetInt( std::string _configName ) const;

private:
	std::map< std::string, int > m_ints;
};