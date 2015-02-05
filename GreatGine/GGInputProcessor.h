#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>
#include <vector>

#include "GGInputHandler.h"

class GGWindow;

class GGInputProcessor
{
public:
	GGInputProcessor( const GGWindow& _window );
	~GGInputProcessor();

public:
	void RegisterHandler( GGInputHandler* _handler );
	void ProcessInput( LPARAM _lParam );

private:
	void SendKebordInput( RAWKEYBOARD& _input );
	void SendMouseInput( RAWMOUSE& _input );

private:
	std::map<USHORT, GG_INPUT> m_keyMap;
	std::vector<GGInputHandler*> m_handlers;
};