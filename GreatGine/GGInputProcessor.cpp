#include "GGInputProcessor.h"
#include "GGWindow.h"
#include "GGError.h"

#include <memory>
using namespace std;

GGInputProcessor::GGInputProcessor( const GGWindow& _window )
	:
	m_keyMap( { { VK_ESCAPE, GG_INPUT_EXIT }, { 'W', GG_INPUT_MOVE_FORWARD }, { 'S', GG_INPUT_MOVE_BACKWARD }, { 'A', GG_INPUT_MOVE_LEFTWARD }, { 'D', GG_INPUT_MOVE_RIGHTWARD } } )
{
	RAWINPUTDEVICE devices[ 2 ];

	// Keyboard
	devices[ 0 ].usUsagePage = 0x01;
	devices[ 0 ].usUsage = 0x06;
	devices[ 0 ].dwFlags = 0;
	devices[ 0 ].hwndTarget = _window.GetHandle();

	//Mouse
	devices[ 1 ].usUsagePage = 0x01;
	devices[ 1 ].usUsage = 0x02;
	devices[ 1 ].dwFlags = 0;
	devices[ 1 ].hwndTarget = _window.GetHandle();

	if( !RegisterRawInputDevices( devices, 2, sizeof( devices[ 0 ] ) ) )
	{
		GG_THROW;
	}
}

GGInputProcessor::~GGInputProcessor()
{
	
}

void GGInputProcessor::RegisterHandler( GGInputHandler* _handler )
{
	m_handlers.push_back( _handler );

	return;
}

void GGInputProcessor::ProcessInput( LPARAM _lParam )
{
	UINT dwSize;

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
	}

	return;
}

void GGInputProcessor::SendKebordInput( RAWKEYBOARD& _input )
{
	bool down = true;
	if( _input.Flags & RI_KEY_BREAK )
	{
		down = false;
	}

	GG_INPUT inputCode;

	try
	{
		inputCode = m_keyMap.at( _input.VKey );
	}
	catch( ... )
	{
		return;
	}

	for( auto handler : m_handlers )
	{
		handler->HandleInput( inputCode, down );
	}

	return;
}

void GGInputProcessor::SendMouseInput( RAWMOUSE& _input )
{

	return;
}