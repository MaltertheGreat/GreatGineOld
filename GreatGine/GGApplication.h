#pragma once

#include "GGInputHandler.h"
#include "GGWindow.h"
#include "GGInput.h"
#include "GGFrameTimer.h"
#include "GGGraphics.h"
#include "GGGame.h"
#include "GGWorld.h"

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
	std::wstring m_title = L"GreatGine";
	std::wstring m_version = L"0.0.2 DEV";

private:
	bool   m_running;
	WPARAM m_exitKey;

	GGWindow         m_window;
	GGInput          m_input;
	GGFrameTimer     m_timer;
	GGGraphics       m_graphics;
	GGGame           m_game;
	GGWorld          m_world;
};