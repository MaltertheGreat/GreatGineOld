#include "PCH.h"
#include "GGInput.h"
#include "GGWindow.h"
#include "GGConfig.h"
#include "GGError.h"

#include <bitset>
using namespace std;

GGInput::GGInput( const GGWindow& _window )
{
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

void GGInput::RegisterHandler( GGInputHandler* _handler )
{
	m_handlers.push_back( _handler );

	return;
}

void GGInput::ProcessInput( const MSG& _msg )
{
	bitset<32> keyflag = _msg.lParam;
	switch( _msg.message )
	{
		case WM_KEYDOWN:
			if( !keyflag[ 30 ] )	// Respond only to first key press. No key repeat
			{
				SendKeyInput( _msg.wParam, true );
			}
			break;
		case WM_KEYUP:
			SendKeyInput( _msg.wParam, false );
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

void GGInput::ProcessRawInput( LPARAM _lParam )
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

		SendMouseInput( mouse.lLastX, mouse.lLastY );
	}

	return;
}

void GGInput::SendKeyInput( WPARAM _keyCode, bool _isDown )
{
	for( auto handler : m_handlers )
	{
		handler->HandleKeyInput( _keyCode, _isDown );
	}

	return;
}

void GGInput::SendMouseInput( int _x, int _y )
{
	for( auto handler : m_handlers )
	{
		handler->HandleMouseInput( _x, _y );
	}

	return;
}