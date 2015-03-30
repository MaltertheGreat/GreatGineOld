#pragma once

#include <array>

#include "GGInputProcessor.h"
#include "GGIWorldViewer.h"
#include "GGFreeCamera.h"
#include "GGChunk.h"

class GGWorld
{
public:
	static const int dimension = 8;

public:
	GGWorld( GGInputProcessor& _inputProcessor );

public:
	void Update( float _frameTime );

	const GGIWorldViewer* GetActiveWorldViewer() const;
	std::array<GGChunk, dimension * dimension>& GetChunkArray();

private:
	void CreateWorld();
	bool RandomlyPopulateChunk( std::vector<std::unique_ptr<GGDepthLevel>>& _depthLevels, UINT _maxDepth = 8, UINT _depth = 0 );

private:
	GGFreeCamera m_freeCamera;

	static const float m_chunkDiameter;
	std::array<GGChunk, dimension * dimension> m_chunks;
	// GGPlayerCamera m_playerCamera;
};