#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GGConfig.h"
#include "GGApplication.h"

int WINAPI WinMain( HINSTANCE _hInstance, HINSTANCE, LPSTR, int )
{
	try
	{
		GGConfig config;
		GGApplication app( _hInstance, config );

		app.Run();
	}
	catch( const std::exception& error )
	{
		MessageBoxA( NULL, error.what(), "An error occured!", MB_ICONERROR );
	}

	return 0;
}