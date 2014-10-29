#pragma once

#include "GGWindow.h"
#include "GGGraphics.h"

class GGApplication
{
public:
	GGApplication( HINSTANCE _hInstance );

public:
	void Run();

private:
	void Update();
	void Render();

private:
	const std::wstring m_title = L"GreatGine";
	const std::wstring m_version = L"0.0.1";
	const UINT m_width = 800;
	const UINT m_heigt = 600;

private:
	GGWindow m_window;
	GGGraphics m_graphics;
};