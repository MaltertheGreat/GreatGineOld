#include "GGWindow.h"
using namespace std;

GGWindow::GGWindow( std::wstring title )
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = GetModuleHandle( NULL );
	wcex.hIcon = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );
	wcex.hCursor = LoadCursor( NULL, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = title.c_str();
	wcex.hIconSm = LoadIcon( wcex.hInstance, MAKEINTRESOURCE( IDI_APPLICATION ) );

	RegisterClassEx( &wcex );
	m_hWnd = CreateWindow( title.c_str(), title.c_str(), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, wcex.hInstance, NULL );

	ShowWindow( m_hWnd, SW_SHOW );

	return;
}

HWND GGWindow::GetHandle() const
{
	return m_hWnd;
}

LRESULT CALLBACK GGWindow::WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch( message )
	{
		case WM_DESTROY:
			PostQuitMessage( 0 );
			break;
		default:
			return DefWindowProc( hWnd, message, wParam, lParam );
	}
	return 0;
}