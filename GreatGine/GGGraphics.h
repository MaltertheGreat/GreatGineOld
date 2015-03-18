#pragma once

#include "GGInputHandler.h"
#include "GGDirectXDriver.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGCamera.h"
#include "GGShader.h"
#include "GGMesh.h"
#include "GGDebugInfo.h"

class GGWindow;
class GGConfig;
class GGWorld;

class GGGraphics : public GGInputHandler
{
public:
	GGGraphics( const GGWindow& _window, const GGConfig& _config );

public:
	void Update( const GGWorld& _world, float _frameTime );
	void Render( const GGWorld& _world );

public:
	virtual void HandleActionInput( GG_ACTION_INPUT _input, bool _down ) override;
	virtual void HandleRangeInput( int _x, int _y ) override;

private:
	void SwitchFillType();

	void Render3D( const GGWorld& _world );
	void Render2D();

private:
	GGRenderer::FILL_TYPE m_currentFillType;

	GGDirectXDriver m_driver;
	GGDevice m_device;
	GGRenderer m_renderer;
	GGCamera m_camera;
	GGShader m_basicShader;
	GGMesh m_mesh;
	GGDebugInfo m_debugInfo;
};