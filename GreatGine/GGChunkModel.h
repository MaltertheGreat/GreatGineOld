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
	void Create( const GGDevice& _device, const GGChunk& _chunk, UINT _lod = 0 );

	const GGMesh* GetMesh() const;
	const DirectX::XMFLOAT4X4& GetTransformation() const;

private:
	void CreateMeshData( GGMeshData& _meshData, const std::vector<GGDepthLevel>& _depthLevels, UINT _lod, UINT _count = 1, const DirectX::XMFLOAT3& _center = { 0.0f, 0.0f, 0.0f } );
	std::array<DirectX::XMFLOAT3, 8> CreateCentersArray( const DirectX::XMFLOAT3& _center, float _voxelRadius );
	void CreateVoxel( GGMeshData& _meshData, const DirectX::XMFLOAT3& _center, float _radius );

private:
	std::unique_ptr<GGMesh> m_mesh;
	DirectX::XMFLOAT4X4 m_transformation;
};