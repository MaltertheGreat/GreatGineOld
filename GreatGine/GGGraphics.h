#pragma once

#include <array>

#include "GGInputHandler.h"
#include "GGDirectXDriver.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGCamera.h"
#include "GGObjectModel.h"
#include "GGShader.h"
#include "GGDebugInfo.h"
#include "GGWorld.h"

class GGWindow;
class GGConfig;

class GGGraphics : public GGInputHandler
{
	typedef std::array<GGObjectModel, GGWorld::DIAMETER * GGWorld::DIAMETER> GGObjectModelArray;
public:
	GGGraphics( const GGWindow& _window, const GGConfig& _config );

public:
	void Update( GGWorld& _world, float _frameTime );
	void Render();

public:
	virtual void HandleActionInput( GG_ACTION_INPUT _input, bool _down ) override;
	virtual void HandleRangeInput( int _x, int _y ) override;

private:
	void SwitchFillType();

	void Render3D();
	void Render2D();

private:
	GGRenderer::FILL_TYPE m_currentFillType;

	GGDirectXDriver m_driver;
	GGDevice m_device;
	GGRenderer m_renderer;
	GGCamera m_camera;
	GGObjectModelArray m_objectModels;
	GGShader m_basicShader;
	GGDebugInfo m_debugInfo;
};