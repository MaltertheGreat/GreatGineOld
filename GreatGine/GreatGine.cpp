#include "PCH.h"
#include "GGConfig.h"
#include "GGApplication.h"

int WINAPI WinMain( _In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int )
{
	try
	{
		GGConfig config;
		GGApplication app( _hInstance, config );

		app.Run();
	}
	catch( const std::exception& error )
	{
		MessageBoxA( NULL, error.what(), "An error occurred!", MB_ICONERROR );
	}

	return 0;
}