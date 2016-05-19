#include "PCH.h"
#include "GGApplication.h"
#include "GGConfig.h"
#include "GGError.h"

GGApplication::GGApplication( HINSTANCE _hInstance, GGConfig& _config )
	:
	m_title( L"GreatGine" ),
	m_version( L"0.1.1 DEV" ),
	m_running( false ),
	m_exitKey( _config.GetUint( "key_exit", VK_ESCAPE ) ),

	m_window( m_title + L" v." + m_version, _hInstance, _config ),
	m_input( m_window ),
	m_graphics( m_window, _config ),
	m_world(),
	m_game( m_input, _config )
{
	m_input.RegisterHandler( this );
	m_input.RegisterHandler( &m_graphics );
}

void GGApplication::Run()
{
	m_timer.Start();
	m_running = true;
	MSG msg;
	while( m_running )
	{
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			if( msg.message == WM_QUIT )
			{
				m_running = false;
			}

			m_input.ProcessInput( msg );

			DispatchMessage( &msg );
		}

		Update();
		Render();
	}

	return;
}

void GGApplication::Update()
{
	m_timer.Update();
	double frameTime = m_timer.GetFrameTime();

	m_graphics.Update( m_world, frameTime );

	m_game.Update( m_world, m_graphics, frameTime );

	m_world.Update();

	return;
}

void GGApplication::Render()
{
	m_graphics.Render();

	return;
}

void GGApplication::HandleKeyInput( WPARAM _keyCode, bool _down )
{
	if( _down && (_keyCode == m_exitKey) )
	{
		m_running = false;
	}

	return;
}

void GGApplication::HandleMouseInput( int, int )
{
	return;
}