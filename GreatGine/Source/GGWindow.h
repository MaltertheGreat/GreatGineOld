#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>

class GGWindow
{
public:
	GGWindow( std::wstring title );

public:
	HWND GetHandle() const;

private:
	static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam );

private:
	HWND m_hWnd;
};