#pragma once

#include "GGInputHandler.h"
#include "GGWindow.h"
#include "GGInputProcessor.h"
#include "GGGraphics.h"

class GGConfig;

class GGApplication : public GGInputHandler
{
public:
	GGApplication( HINSTANCE _hInstance, const GGConfig& _config );

public:
	void Run();

private:
	void Update();
	void Render();

	virtual void HandleInput( GG_INPUT _input, bool _down ) override;

private:
	const std::wstring m_title = L"GreatGine";
	const std::wstring m_version = L"0.0.1 DEV";

private:
	bool m_running = false;

	GGWindow m_window;
	GGInputProcessor m_input;
	GGGraphics m_graphics;
};