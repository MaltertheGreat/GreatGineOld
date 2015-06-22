#include "PCH.h"
#include "GGWorld.h"

#include <random>
using namespace DirectX;
using namespace std;

array<XMFLOAT3, 1> test{ { { 0.0f, 0.0f, 0.0f } } };

GGWorld::GGWorld()
	:
	m_chunks( InitializeChunks() )
{}

void GGWorld::Update()
{
	for( auto& chunk : m_chunks )
	{
		chunk.Update();

		if( chunk.GetState() == GGChunk::GG_CHUNK_STATE_UNGENERATED )
		{
			GenerateChunk( chunk );
		}
	}

	return;
}

void GGWorld::SetCamera( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation )
{
	m_cameraPosition = _position;
	m_cameraRotation = _rotation;

	return;
}

GGWorld::GGChunkArray& GGWorld::GetChunkArray()
{
	return m_chunks;
}

const DirectX::XMFLOAT3 & GGWorld::GetCameraPosition() const
{
	return m_cameraPosition;
}

const DirectX::XMFLOAT3 & GGWorld::GetCameraRotation() const
{
	return m_cameraRotation;
}

GGWorld::GGChunkArray GGWorld::InitializeChunks()
{
	GGChunkArray chunks;

	const float chunkOffset = DIAMETER / 2.0f * GGChunk::DIMENSION - GGChunk::DIMENSION * 0.5f;
	float x = -chunkOffset;
	float z = -chunkOffset;

	for( auto& chunk : chunks )
	{
		chunk = GGChunk( { x, 0.0f, z } );

		XMFLOAT3 position = { x, 0.0f, z };
		z += GGChunk::DIMENSION;
		if( z > chunkOffset )
		{
			z = -chunkOffset;
			x += GGChunk::DIMENSION;
		}
	}

	return chunks;
}

void GGWorld::GenerateChunk( GGChunk& _chunk )
{
	float voxelDimension = GGChunk::DIMENSION / static_cast<float>(GGObject::DIAMETER);
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	GGObject object( move( CreateRandomVoxels() ), voxelDimension, position );

	_chunk.AddObject( move( object ) );

	static default_random_engine gen;
	static bernoulli_distribution solid( 0.1 );
	if( solid( gen ) )
	{
		voxelDimension /= 4.0f;
		position = { 0.0f, GGChunk::DIMENSION / 2.0f, 0.0f };
		GGObject smallObject( move( CreateRandomVoxels() ), voxelDimension, position );

		_chunk.AddObject( move( smallObject ) );
	}

	_chunk.SetState( GGChunk::GG_CHUNK_STATE_GENERAETD );

	return;
}

GGObject::GGVoxelArray GGWorld::CreateRandomVoxels()
{
	static default_random_engine gen;
	static bernoulli_distribution solid( 0.025 );

	GGObject::GGVoxelArray voxels;
	for( auto& voxel : voxels )
	{
		if( solid( gen ) )
		{
			voxel.element = 1;
		}
	}

	return voxels;
}