#pragma once

#include <array>
#include <bitset>

#include "GGInputHandler.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGChunkModelSet.h"
#include "GGShader.h"
#include "GGDebugInfo.h"
#include "GGWorld.h"

class GGWindow;
class GGConfig;

class GGGraphics : public GGInputHandler
{
private:
	enum GG_KEYMAP
	{
		GG_KEYMAP_FILL_TYPE = 0,
		GG_KEYMAP_RENDER_CHUNKS,

		GG_KEYMAP_COUNT
	};
	enum GG_RENDER_FLAGS
	{
		GG_RENDER_FLAGS_DEBUG = 0,
		GG_RENDER_FLAGS_WORLD,
		GG_RENDER_FLAGS_GUI,

		GG_RENDER_FLAGS_COUNT
	};

	typedef std::array<GGChunkModelSet, GGWorld::DIAMETER * GGWorld::DIAMETER> GGChunkModelSets;

public:
	GGGraphics( const GGWindow& _window, GGConfig& _config );

public:
	void Update( GGWorld& _world, double _timeDelta );
	void Render();

	UINT GetResolutionX() const;
	UINT GetResolutionY() const;

public:
	virtual void HandleKeyInput( WPARAM _keyCode, bool _down ) override;
	virtual void HandleMouseInput( int _x, int _y ) override;

private:
	void SwitchFillType();

	void Render3D();
	void Render2D();

private:
	UINT     m_resolutionX;
	UINT     m_resolutionY;
	WPARAM   m_keyMap[ GG_KEYMAP_COUNT ];

	GGRenderer::FILL_TYPE              m_currentFillType;
	std::bitset<GG_RENDER_FLAGS_COUNT> m_renderFlags;

	GGDevice         m_device;
	GGRenderer       m_renderer;
	GGChunkModelSets m_chunkModelSets;
	GGShader         m_basicShader;
	GGDebugInfo      m_debugInfo;
	GGMesh           m_debugChunkMesh;
	GGShader         m_debugChunkShader;
};