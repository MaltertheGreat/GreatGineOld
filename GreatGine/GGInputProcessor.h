#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <map>
#include <vector>

#include "GGInputHandler.h"

class GGWindow;
class GGConfig;

class GGInputProcessor
{
public:
	static const UINT INPUT_MESSAGE = WM_KEYDOWN | WM_KEYUP | WM_CHAR | WM_INPUT;

public:
	GGInputProcessor( const GGWindow& _window, const GGConfig& _config );

public:
	void RegisterHandler( GGInputHandler* _handler );
	void ProcessInput( const MSG& _msg );

private:
	void ProcessKeyPress( WPARAM _wParam, LPARAM _lParam );
	void ProcessKeyRelease( WPARAM _wParam );
	void ProcessRawInput( LPARAM _lParam );
	void SendActionInput( GG_ACTION_INPUT _input, bool _down );
	void SendRangeInput( int _x, int _y );

private:
	UINT m_actionKeysMap[ GG_ACTION_INPUT_COUNT ];
	std::vector<GGInputHandler*> m_handlers;
};