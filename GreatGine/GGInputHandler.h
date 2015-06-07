#pragma once

class GGInputHandler
{
public:
	virtual void HandleKeyInput( WPARAM _keyCode, bool _down ) = 0;
	virtual void HandleMouseInput( int _x, int _y ) = 0;
};