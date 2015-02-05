#include "GGApplication.h"
#include "GGConfig.h"
#include "GGError.h"


GGApplication::GGApplication( HINSTANCE _hInstance, const GGConfig& _config )
	:
	m_window( m_title + L" v." + m_version, _hInstance, _config ),
	m_input( m_window ),
	m_graphics( m_window )
{
	m_input.RegisterHandler( this );
	m_input.RegisterHandler( &m_graphics );
}

void GGApplication::Run()
{
	m_running = true;
	MSG msg;
	while( m_running )
	{
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			switch( msg.message )
			{
			case WM_QUIT:
				m_running = false;
				break;
			case WM_INPUT:
				m_input.ProcessInput( msg.lParam );
				DefWindowProc( msg.hwnd, msg.message, msg.wParam, msg.lParam );
				break;
			}

			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		Update();
		Render();
	}

	return;
}

void GGApplication::Update()
{
	m_graphics.Update();

	return;
}

void GGApplication::Render()
{
	m_graphics.Render();

	return;
}

void GGApplication::HandleInput( GG_INPUT _input, bool _down )
{
	if( _down && (_input == GG_INPUT_EXIT) )
	{
		m_running = false;
	}
}
