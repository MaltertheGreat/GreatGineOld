#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GGApplication.h"

int WINAPI WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow )
{
	try
	{
		GGApplication app( _hInstance );

		app.Run();
	}
	catch( const std::exception& error )
	{
		MessageBoxA( NULL, error.what(), "An error occured!", MB_ICONERROR );
	}

	return 0;
}