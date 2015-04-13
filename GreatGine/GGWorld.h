#pragma once

#include <array>
#include <memory>

#include "GGInputProcessor.h"
#include "GGIWorldViewer.h"
#include "GGFreeCamera.h"
#include "GGChunk.h"

class GGWorld
{
public:
	static const UINT DIMENSION = 32;
	typedef std::array<std::unique_ptr<GGChunk>, DIMENSION * DIMENSION> GGChunkArray;

public:
	GGWorld( GGInputProcessor& _inputProcessor );

public:
	void Update( float _frameTime );

	const GGIWorldViewer* GetActiveWorldViewer() const;
	GGChunkArray& GetChunkArray();

private:
	void CreateWorld();
	GGChunk::GGVoxelArray CreateRandomVoxels();

private:
	GGFreeCamera m_freeCamera;

	static const float m_chunkDiameter;
	GGChunkArray m_chunks;
};