#include "PCH.h"
#include "GGInputProcessor.h"
#include "GGWindow.h"
#include "GGConfig.h"
#include "GGError.h"

#include <bitset>
#include <assert.h>
using namespace std;

GGInputProcessor::GGInputProcessor( const GGWindow& _window, const GGConfig& _config )
{
	m_actionKeysMap[ GG_ACTION_INPUT_EXIT ] = _config.GetInt( "key_exit" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_FORWARD ] = _config.GetInt( "key_forward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_BACKWARD ] = _config.GetInt( "key_backward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_RIGHTWARD ] = _config.GetInt( "key_rightward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_LEFTWARD ] = _config.GetInt( "key_leftward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_UPWARD ] = _config.GetInt( "key_upward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_DOWNWARD ] = _config.GetInt( "key_downward" );
	m_actionKeysMap[ GG_ACTION_INPUT_FILL_TYPE ] = _config.GetInt( "key_fill_type" );
	m_actionKeysMap[ GG_ACTION_INPUT_RENDER_CHUNKS ] = _config.GetInt( "key_render_chunks" );

	RAWINPUTDEVICE mouse;
	mouse.usUsagePage = 0x01;
	mouse.usUsage = 0x02;
	mouse.dwFlags = RIDEV_CAPTUREMOUSE | RIDEV_NOLEGACY;
	mouse.hwndTarget = _window.GetHandle();

	if( !RegisterRawInputDevices( &mouse, 1, sizeof( mouse ) ) )
	{
		GG_THROW;
	}
}

void GGInputProcessor::RegisterHandler( GGInputHandler* _handler )
{
	m_handlers.push_back( _handler );

	return;
}

void GGInputProcessor::ProcessInput( const MSG& _msg )
{
	switch( _msg.message )
	{
		case WM_KEYDOWN:
			ProcessKeyPress( _msg.wParam, _msg.lParam );
			break;
		case WM_KEYUP:
			ProcessKeyRelease( _msg.wParam );
			break;
		case WM_CHAR:
			break;
		case WM_INPUT:
			ProcessRawInput( _msg.lParam );
			break;
	}

	TranslateMessage( &_msg );

	return;
}

void GGInputProcessor::ProcessKeyPress( WPARAM _wParam, LPARAM _lParam )
{
	bitset<32> keyflag = _lParam;

	if( !keyflag[ 30 ] )	// Respond only to first key press. No key repeat
	{
		for( int i = 0; i < GG_ACTION_INPUT_COUNT; ++i )
		{
			if( m_actionKeysMap[ i ] == _wParam )
			{
				SendActionInput( static_cast<GG_ACTION_INPUT>(i), true );
			}
		}
	}

	return;
}

void GGInputProcessor::ProcessKeyRelease( WPARAM _wParam )
{
	for( int i = 0; i < GG_ACTION_INPUT_COUNT; ++i )
	{
		if( m_actionKeysMap[ i ] == _wParam )
		{
			SendActionInput( static_cast<GG_ACTION_INPUT>(i), false );
		}
	}

	return;
}

void GGInputProcessor::ProcessRawInput( LPARAM _lParam )
{
	UINT dwSize;

	GetRawInputData( (HRAWINPUT)_lParam, RID_INPUT, NULL, &dwSize, sizeof( RAWINPUTHEADER ) );

	unique_ptr<BYTE[]> buffer( new BYTE[ dwSize ] );

	if( GetRawInputData( (HRAWINPUT)_lParam, RID_INPUT, buffer.get(), &dwSize,
						 sizeof( RAWINPUTHEADER ) ) != dwSize )
	{
		GG_THROW;
	}

	RAWINPUT* input = (RAWINPUT*)buffer.get();

	if( input->header.dwType == RIM_TYPEMOUSE )
	{
		RAWMOUSE mouse = input->data.mouse;

		assert( (mouse.usFlags & MOUSE_MOVE_RELATIVE) == 0 );	// For simplicity temporarily assume relative mouse movement

		SendRangeInput( mouse.lLastX, mouse.lLastY );
	}

	return;
}

void GGInputProcessor::SendActionInput( GG_ACTION_INPUT _input, bool _down )
{
	for( auto handler : m_handlers )
	{
		handler->HandleActionInput( _input, _down );
	}

	return;
}

void GGInputProcessor::SendRangeInput( int _x, int _y )
{
	for( auto handler : m_handlers )
	{
		handler->HandleRangeInput( _x, _y );
	}

	return;
}