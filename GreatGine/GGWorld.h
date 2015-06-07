#pragma once

#include <array>
#include <memory>

#include "GGIWorldViewer.h"
#include "GGFreeCamera.h"
#include "GGChunk.h"

class GGInput;
class GGConfig;

class GGWorld
{
public:
	static constexpr UINT DIAMETER = 4;
	typedef std::array<GGChunk, DIAMETER * DIAMETER> GGChunkArray;

public:
	GGWorld( GGInput& _inputProcessor, GGConfig& _config );

public:
	void Update( float _frameTime );

	const GGIWorldViewer* GetActiveWorldViewer() const;
	GGChunkArray& GetChunkArray();

private:
	// TODO: Move these functions to some sort of world generator
	static GGChunkArray InitializeChunks();
	static void GenerateChunk( GGChunk& _chunk );
	static GGObject::GGVoxelArray CreateRandomVoxels();

private:
	GGFreeCamera m_freeCamera;

	GGChunkArray m_chunks;
};