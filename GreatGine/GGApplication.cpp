#include "GGApplication.h"
#include "GGError.h"


GGApplication::GGApplication( HINSTANCE _hInstance )
	:
	m_window( m_title + L" v." + m_version, m_width, m_heigt, _hInstance ),
	m_graphics( m_window )
{}

void GGApplication::Run()
{
	bool running = true;
	MSG msg;
	while( running )
	{
		while( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );

			switch( msg.message )
			{
			case WM_QUIT:
				running = false;
				break;
			}
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