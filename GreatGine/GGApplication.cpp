#include "PCH.h"
#include "GGApplication.h"
#include "GGConfig.h"
#include "GGError.h"

GGApplication::GGApplication( HINSTANCE _hInstance, const GGConfig& _config )
	:
	m_window( m_title + L" v." + m_version, _hInstance, _config ),
	m_input( m_window, _config ),
	m_graphics( m_window, _config ),
	m_world( m_input )
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
			else if( msg.message & GGInputProcessor::INPUT_MESSAGE )
			{
				m_input.ProcessInput( msg );
			}

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
	float frameTime = m_timer.GetFrameTime();

	m_world.Update( frameTime );
	m_graphics.Update( m_world, frameTime );

	return;
}

void GGApplication::Render()
{
	m_graphics.Render();

	return;
}

void GGApplication::HandleActionInput( GG_ACTION_INPUT _input, bool _down )
{
	if( _down && (_input == GG_ACTION_INPUT_EXIT) )
	{
		m_running = false;
	}

	return;
}

void GGApplication::HandleRangeInput( int, int )
{
	return;
}