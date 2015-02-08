#include "GGInputProcessor.h"
#include "GGWindow.h"
#include "GGConfig.h"
#include "GGError.h"

#include <bitset>
#include <memory>
using namespace std;

GGInputProcessor::GGInputProcessor( const GGWindow& _window, const GGConfig& _config )
{
	m_actionKeysMap[ GG_ACTION_INPUT_EXIT ] = _config.GetInt( "key_exit" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_FORWARD ] = _config.GetInt( "key_forward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_BACKWARD ] = _config.GetInt( "key_backward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_RIGHTWARD ] = _config.GetInt( "key_rightward" );
	m_actionKeysMap[ GG_ACTION_INPUT_MOVE_LEFTWARD ] = _config.GetInt( "key_leftward" );

	RAWINPUTDEVICE mouse;
	mouse.usUsagePage = 0x01;
	mouse.usUsage = 0x02;
	mouse.dwFlags = 0;
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
	bitset<32> keyflag = _msg.lParam;

	switch( _msg.message )
	{
	case WM_KEYDOWN:
		if( !keyflag[ 30 ] )	// Respond only to first key press. No key repeat
		{
			for( int i = 0; i < GG_ACTION_INPUT_COUNT; ++i )
			{
				if( m_actionKeysMap[ i ] == _msg.wParam )
				{
					SendActionInput( static_cast<GG_ACTION_INPUT>(i), true );
				}
			}
		}
		break;
	case WM_KEYUP:
		for( int i = 0; i < GG_ACTION_INPUT_COUNT; ++i )
		{
			if( m_actionKeysMap[ i ] == _msg.wParam )
			{
				SendActionInput( static_cast<GG_ACTION_INPUT>(i), false );
			}
		}
		break;
	case WM_CHAR:
		break;
	case WM_INPUT:
		break;
	}

	TranslateMessage( &_msg );

	/*UINT dwSize;

	GetRawInputData( (HRAWINPUT) _lParam, RID_INPUT, NULL, &dwSize, sizeof( RAWINPUTHEADER ) );

	unique_ptr<BYTE[]> buffer( new BYTE[ dwSize ] );

	if( GetRawInputData( (HRAWINPUT) _lParam, RID_INPUT, buffer.get(), &dwSize,
	sizeof( RAWINPUTHEADER ) ) != dwSize )
	{
	GG_THROW;
	}

	RAWINPUT* input = (RAWINPUT*) buffer.get();

	if( input->header.dwType == RIM_TYPEKEYBOARD )
	{
	SendKebordInput( input->data.keyboard );
	}
	else if( input->header.dwType == RIM_TYPEMOUSE )
	{
	SendMouseInput( input->data.mouse );
	}*/

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