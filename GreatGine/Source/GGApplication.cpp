#include "GGApplication.h"

GGApplication::GGApplication()
	:
	m_window( m_title + L" v." + m_version )
{
}

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
	}

	return;
}