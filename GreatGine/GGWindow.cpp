#include "GGWindow.h"
#include "GGConfig.h"
using namespace std;

GGWindow::GGWindow( std::wstring title, HINSTANCE _hInstance, const GGConfig& _config )
	:
	m_width( _config.GetInt( "window_width" ) ),
	m_height( _config.GetInt( "window_height" ) )
{
	WNDCLASSEX wcex = { 0 };

	wcex.cbSize = sizeof( WNDCLASSEX );
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &WndProc;
	wcex.hInstance = _hInstance;
	wcex.hCursor = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = title.c_str();

	RegisterClassEx( &wcex );

	// Convert client size to window size
	RECT windowRect = { 0, 0, m_width, m_height };
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;
	AdjustWindowRect( &windowRect, windowStyle, FALSE );
	UINT windowWidth = windowRect.right - windowRect.left;
	UINT windowHeigth = windowRect.bottom - windowRect.top;

	m_hWnd = CreateWindow( title.c_str(), title.c_str(), windowStyle, CW_USEDEFAULT, CW_USEDEFAULT, windowWidth, windowHeigth, nullptr, nullptr, _hInstance, nullptr );

	ShowWindow( m_hWnd, SW_SHOW );
	UpdateWindow( m_hWnd );

	return;
}

HWND GGWindow::GetHandle() const
{
	return m_hWnd;
}

UINT GGWindow::GetWidth() const
{
	return m_width;
}

UINT GGWindow::GetHeight() const
{
	return m_height;
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