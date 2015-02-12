#pragma once

enum GG_ACTION_INPUT
{
	GG_ACTION_INPUT_EXIT,

	GG_ACTION_INPUT_MOVE_FORWARD,
	GG_ACTION_INPUT_MOVE_BACKWARD,
	GG_ACTION_INPUT_MOVE_RIGHTWARD,
	GG_ACTION_INPUT_MOVE_LEFTWARD,
	GG_ACTION_INPUT_FILL_TYPE,

	GG_ACTION_INPUT_COUNT
};

class GGInputHandler
{
public:
	virtual void HandleActionInput( GG_ACTION_INPUT _input, bool _down ) = 0;
	virtual void HandleRangeInput( int _x, int _y ) = 0;
};