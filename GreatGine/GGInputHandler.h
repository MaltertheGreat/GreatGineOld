#pragma once

enum GG_INPUT
{
	GG_INPUT_EXIT = 0,

	GG_INPUT_MOVE_FORWARD,
	GG_INPUT_MOVE_BACKWARD,
	GG_INPUT_MOVE_RIGHTWARD,
	GG_INPUT_MOVE_LEFTWARD,

	GG_INPUT_COUNT
};

class GGInputHandler
{
public:
	virtual void HandleInput( GG_INPUT _input, bool _down )
	{
		return;
	}

	virtual void HandleMouseMove()
	{
		return;
	}
};