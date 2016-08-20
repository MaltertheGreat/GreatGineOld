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

	struct GGVoxelFaceDescription
	{
		GGVoxelDescription voxel;
		GGVoxel::GG_VOXEL_FACE face;
	};

public:
	void Update();

	void SetViewPoint( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation );

	GGChunk& GetChunk( GGChunkDescription _desc );
	GGChunkArray& GetChunkArray();

	const GGChunk& GetChunk( GGChunkDescription _desc )const;
	const GGChunkArray& GetChunkArray() const;
	const DirectX::XMFLOAT3& GetViewPointPosition() const;
	const DirectX::XMFLOAT3& GetViewPointRotation() const;

	std::unique_ptr<GGVoxelFaceDescription> GetVoxelFromRay( UINT _originChunkX, UINT _originChunkZ, const DirectX::XMFLOAT3& _originPosition, const DirectX::XMFLOAT3& _rotation, float _length, GGObjectDescription* _excludedObject = nullptr );

	void PlaceVoxelOn( GGVoxelFaceDescription _voxelFace );

	const static UINT ConvertCoordsToId( const DirectX::XMUINT3& _coords, const DirectX::XMUINT3& _diameter );

private:
	// TODO: Move these functions to some sort of world generator
	static void GenerateChunk( GGChunk& _chunk );
	static GGObject::GGVoxels CreateRandomVoxels();

	std::vector<std::pair<GGChunkDescription, DirectX::XMFLOAT3 >> ChunksToCheckRay( UINT _originChunkX, UINT _originChunkZ, const DirectX::XMFLOAT3& _originPosition );
	std::pair<std::unique_ptr<GGVoxelFaceDescription>, float> GetVoxelFromRayInObject( const DirectX::XMFLOAT3& _originPos, const DirectX::XMFLOAT3& _ray, float _length, const GGObject& _object );

private:
	GGChunkArray m_chunks;
	DirectX::XMFLOAT3 m_viewPointPosition;
	DirectX::XMFLOAT3 m_viewPointRotation;
};

bool operator ==( const GGWorld::GGChunkDescription& a, const GGWorld::GGChunkDescription& b );
bool operator ==( const GGWorld::GGObjectDescription& a, const GGWorld::GGObjectDescription& b );