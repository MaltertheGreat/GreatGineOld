#include "PCH.h"
#include "GGWorld.h"
#include <random>
using namespace DirectX;
using namespace std;

array<XMFLOAT3, 1> test{ { { 0.0f, 0.0f, 0.0f } } };

GGWorld::GGWorld( GGInputProcessor& _inputProcessor )
	:
	m_freeCamera( { 0.0f, 10.0f, -5.0f } ),
	m_chunks( GenerateChunks() )
{
	_inputProcessor.RegisterHandler( &m_freeCamera );
}

void GGWorld::Update( float _frameTime )
{
	m_freeCamera.Update( _frameTime );

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

const GGIWorldViewer* GGWorld::GetActiveWorldViewer() const
{
	return dynamic_cast<const GGIWorldViewer*>(&m_freeCamera);
}

GGWorld::GGChunkArray& GGWorld::GetChunkArray()
{
	return m_chunks;
}

GGWorld::GGChunkArray GGWorld::GenerateChunks()
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

void GGWorld::GenerateChunk( GGChunk & _chunk )
{
	float voxelDimension = GGChunk::DIMENSION / static_cast<float>(GGObject::DIAMETER);
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	GGObject object( move( CreateRandomVoxels() ), voxelDimension, position );

	_chunk.AddObject( move( object ) );
	_chunk.SetState( GGChunk::GG_CHUNK_STATE_GENERAETD );

	return;
}

GGObject::GGVoxelArray GGWorld::CreateRandomVoxels()
{
	static default_random_engine gen;
	static bernoulli_distribution solid( 0.1 );

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