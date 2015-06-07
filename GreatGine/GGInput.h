#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <vector>

#include "GGInputHandler.h"

class GGWindow;
class GGConfig;

class GGInput
{
public:
	GGInput( const GGWindow& _window );

public:
	void RegisterHandler( GGInputHandler* _handler );
	void ProcessInput( const MSG& _msg );

private:
	void ProcessRawInput( LPARAM _lParam );

	void SendKeyInput( WPARAM _keyCode, bool _isDown );
	void SendMouseInput( int _x, int _y );

private:
	std::vector<GGInputHandler*> m_handlers;
};