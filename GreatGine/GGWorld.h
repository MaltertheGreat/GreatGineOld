#pragma once

#include <array>
#include <memory>

#include "GGChunk.h"

class GGWorld
{
public:
	static constexpr UINT DIAMETER = 4;
	typedef std::array<GGChunk, DIAMETER * DIAMETER> GGChunkArray;

	struct GGVoxelDescription
	{
		UINT chunkX;
		UINT chunkZ;
		GGChunk::GGObjectID objectID;
		UINT voxelX;
		UINT voxelY;
		UINT voxelZ;
		GGVoxel::GG_VOXEL_FACE face;
	};

public:
	GGWorld();

public:
	void Update();

	void SetRenderable( bool _renderable );
	void SetViewPoint( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation );

	bool IsRenderable() const;
	GGChunk& GetChunk( UINT _x, UINT _z );
	GGChunkArray& GetChunkArray();
	const DirectX::XMFLOAT3& GetViewPointPosition() const;
	const DirectX::XMFLOAT3& GetViewPointRotation() const;

	std::unique_ptr<GGVoxelDescription> GetVoxelFromRay( UINT _originChunkX, UINT _originChunkZ, const DirectX::XMFLOAT3& _originPositin, const DirectX::XMFLOAT3& _rotation, float _length, GGChunk::GGObjectID* _excludedObject = nullptr );

private:
	// TODO: Move these functions to some sort of world generator
	static GGChunkArray InitializeChunks();
	static void GenerateChunk( GGChunk& _chunk );
	static GGObject::GGVoxelArray CreateRandomVoxels();

	std::unique_ptr<GGVoxelDescription> GetVoxelFromRayInObject( const DirectX::XMFLOAT3& _originPositin, const DirectX::XMFLOAT3& _rotation, float _length );

private:
	bool m_renderable;
	GGChunkArray m_chunks;
	DirectX::XMFLOAT3 m_viewPointPosition;
	DirectX::XMFLOAT3 m_viewPointRotation;
};