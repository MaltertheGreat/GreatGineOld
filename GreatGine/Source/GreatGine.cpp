#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GGApplication.h"

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
{
	GGApplication app;

	app.Run();

	return 0;
}