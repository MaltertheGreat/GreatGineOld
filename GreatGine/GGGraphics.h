#pragma once

#include <array>

#include "GGInputHandler.h"
#include "GGDirectXDriver.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGCamera.h"
#include "GGChunkModel.h"
#include "GGShader.h"
#include "GGDebugInfo.h"

class GGWindow;
class GGConfig;
class GGWorld;

class GGGraphics : public GGInputHandler
{
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
	static const UINT m_dimension = 1;

	GGDirectXDriver m_driver;
	GGDevice m_device;
	GGRenderer m_renderer;
	GGCamera m_camera;
	std::array<GGChunkModel, m_dimension * m_dimension> m_chunkModels;
	GGShader m_basicShader;
	GGDebugInfo m_debugInfo;
};