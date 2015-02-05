#pragma once

#include <map>

class GGConfig
{
public:
	GGConfig();

public:
	char GetChar( std::string _configName ) const;
	int GetInt( std::string _configName ) const;

private:
	std::map< std::string, char > m_chars;
	std::map< std::string, int > m_ints;
};