#pragma once

#include <array>

#include "GGInputHandler.h"
#include "GGDirectXDriver.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGCamera.h"
#include "GGChunkModelSet.h"
#include "GGShader.h"
#include "GGDebugInfo.h"
#include "GGWorld.h"

class GGWindow;
class GGConfig;

class GGGraphics : public GGInputHandler
{
	typedef std::array<GGChunkModelSet, GGWorld::DIAMETER * GGWorld::DIAMETER> GGChunkModelSets;
public:
	GGGraphics( const GGWindow& _window, const GGConfig& _config );

public:
	void Update( GGWorld& _world, float _frameTime );
	void Render();

public:
	virtual void HandleActionInput( GG_ACTION_INPUT _input, bool _down ) override;
	virtual void HandleRangeInput( int _x, int _y ) override;

private:
	static GGLinesData VerticalLine();
	void SwitchFillType();

	void Render3D();
	void Render2D();

private:
	GGRenderer::FILL_TYPE m_currentFillType;
	bool m_renderChunks;

	GGDirectXDriver  m_driver;
	GGDevice         m_device;
	GGRenderer       m_renderer;
	GGCamera         m_camera;
	GGChunkModelSets m_chunkModelSets;
	GGShader         m_basicShader;
	GGDebugInfo      m_debugInfo;
	GGMesh           m_debugChunkMesh;
	GGShader         m_debugChunkShader;
};