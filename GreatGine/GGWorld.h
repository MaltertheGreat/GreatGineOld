#pragma once

#include <array>
#include <memory>
#include <utility>

#include "GGChunk.h"

class GGWorld
{
public:
	static constexpr UINT DIAMETER = 4;
	typedef std::array<GGChunk, DIAMETER * DIAMETER> GGChunkArray;

	struct GGChunkDescription
	{
		UINT chunkX;
		UINT chunkZ;
	};

	struct GGObjectDescription
	{
		GGChunkDescription chunk;
		GGChunk::GGObjectID objectID;
	};

	struct GGVoxelDescription
	{
		GGObjectDescription object;
		UINT voxelX;
		UINT voxelY;
		UINT voxelZ;
	};

	/*struct GGVoxelDescription
	{
		UINT chunkX;
		UINT chunkZ;
		GGChunk::GGObjectID objectID;
		UINT voxelX;
		UINT voxelY;
		UINT voxelZ;
		GGVoxel::GG_VOXEL_FACE face;
	};*/

	struct GGVoxelFaceDescription
	{
		GGVoxelDescription voxel;
		GGVoxel::GG_VOXEL_FACE face;
	};

public:
	GGWorld();

public:
	void Update();

	void SetRenderable( bool _renderable );
	void SetViewPoint( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation );

	bool IsRenderable() const;
	GGChunk& GetChunk( GGChunkDescription _desc );
	GGChunk& GetChunk( UINT _x, UINT _z );
	GGChunkArray& GetChunkArray();
	const DirectX::XMFLOAT3& GetViewPointPosition() const;
	const DirectX::XMFLOAT3& GetViewPointRotation() const;

	std::unique_ptr<GGVoxelFaceDescription> GetVoxelFromRay( UINT _originChunkX, UINT _originChunkZ, const DirectX::XMFLOAT3& _originPosition, const DirectX::XMFLOAT3& _rotation, float _length, GGObjectDescription* _excludedObject = nullptr );

private:
	// TODO: Move these functions to some sort of world generator
	static GGChunkArray InitializeChunks();
	static void GenerateChunk( GGChunk& _chunk );
	static GGObject::GGVoxelArray CreateRandomVoxels();

	std::pair<std::unique_ptr<GGVoxelFaceDescription>, float> GetVoxelFromRayInObject( const DirectX::XMFLOAT3& _originPos, const DirectX::XMFLOAT3& _ray, float _length, const GGObject& _object );

private:
	bool m_renderable;
	GGChunkArray m_chunks;
	DirectX::XMFLOAT3 m_viewPointPosition;
	DirectX::XMFLOAT3 m_viewPointRotation;
};

bool operator ==( const GGWorld::GGChunkDescription& a, const GGWorld::GGChunkDescription& b );
bool operator ==( const GGWorld::GGObjectDescription& a, const GGWorld::GGObjectDescription& b );