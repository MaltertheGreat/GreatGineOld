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
	static constexpr UINT DIAMETER = 8;
	typedef std::array<GGChunk, DIAMETER * DIAMETER> GGChunkArray;

public:
	GGWorld( GGInputProcessor& _inputProcessor );

public:
	void Update( float _frameTime );

	const GGIWorldViewer* GetActiveWorldViewer() const;
	GGChunkArray& GetChunkArray();

private:
	// TODO: Move these functions to some sort of world generator
	static GGChunkArray GenerateChunks();
	void GenerateChunk( GGChunk& _chunk );
	GGObject::GGVoxelArray CreateRandomVoxels();

private:
	GGFreeCamera m_freeCamera;

	GGChunkArray       m_chunks;
};