#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GGApplication.h"

int WINAPI WinMain( HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow )
{
	GGApplication app( _hInstance );

	app.Run();

	return 0;
}