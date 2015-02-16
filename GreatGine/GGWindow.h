#pragma once

#include "PCH.h"

class GGConfig;

class GGWindow
{
public:
	GGWindow( std::wstring title, HINSTANCE _hInstance, const GGConfig& _config );

public:
	HWND GetHandle() const;
	// Client area width
	UINT GetWidth() const;
	// Client area height
	UINT GetHeight() const;

private:
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
	HWND m_hWnd;
	UINT m_width;
	UINT m_height;
};