#pragma once

#include "PCH.h"
#include "GGRenderer.h"
#include "GGMesh.h"
#include "GGChunk.h"
#include "GGMeshData.h"

class GGDevice;

class GGChunkModel
{
public:
	void Create( const GGDevice& _device, const GGChunk& _chunk );

	const GGMesh* GetMesh() const;
	const DirectX::XMFLOAT4X4& GetTransformation() const;

private:
	GGMeshData CreateChunkGrid( const GGChunk& _chunk );
	void CreateVoxel( UINT x, UINT y, UINT z, const GGChunk::GGVoxelArray& voxels, GGMeshData& _grid );

private:
	std::unique_ptr<GGMesh> m_mesh;
	DirectX::XMFLOAT4X4 m_transformation;
};