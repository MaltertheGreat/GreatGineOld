#pragma once

#include <array>
#include <memory>

#include "GGChunk.h"

class GGWorld
{
public:
	static constexpr UINT DIAMETER = 4;
	typedef std::array<GGChunk, DIAMETER * DIAMETER> GGChunkArray;

public:
	GGWorld();

public:
	void Update();

	void SetCamera( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation );

	GGChunkArray& GetChunkArray();
	const DirectX::XMFLOAT3& GetCameraPosition() const;
	const DirectX::XMFLOAT3& GetCameraRotation() const;

private:
	// TODO: Move these functions to some sort of world generator
	static GGChunkArray InitializeChunks();
	static void GenerateChunk( GGChunk& _chunk );
	static GGObject::GGVoxelArray CreateRandomVoxels();

private:
	GGChunkArray m_chunks;
	DirectX::XMFLOAT3 m_cameraPosition;
	DirectX::XMFLOAT3 m_cameraRotation;
};