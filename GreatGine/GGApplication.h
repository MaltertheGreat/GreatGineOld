#pragma once

#include "GGInputHandler.h"
#include "GGWindow.h"
#include "GGInput.h"
#include "GGFrameTimer.h"
#include "GGGraphics.h"
#include "GGWorld.h"
#include "GGGame.h"

class GGConfig;

class GGApplication : public GGInputHandler
{
public:
	GGApplication( HINSTANCE _hInstance, GGConfig& _config );

public:
	void Run();

private:
	void Update();
	void Render();

public:
	virtual void HandleKeyInput( WPARAM _keyCode, bool _down ) override;
	virtual void HandleMouseInput( int _x, int _y ) override;

private:
	std::wstring m_title;
	std::wstring m_version;

private:
	bool m_running;
	WPARAM m_exitKey;

	GGWindow m_window;
	GGInput m_input;
	GGFrameTimer m_timer;
	GGGraphics m_graphics;
	GGWorld m_world;
	GGGame m_game;
};