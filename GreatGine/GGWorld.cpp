#include "PCH.h"
#include "GGWorld.h"
#include <random>
using namespace DirectX;
using namespace std;

const float GGWorld::m_chunkDiameter = 16.0f;

GGWorld::GGWorld( GGInputProcessor& _inputProcessor )
	:
	m_freeCamera( { 0.0f, 10.0f, -5.0f } )
{
	_inputProcessor.RegisterHandler( &m_freeCamera );

	CreateWorld();
}

void GGWorld::Update( float _frameTime )
{
	m_freeCamera.Update( _frameTime );

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

void GGWorld::CreateWorld()
{
	float chunkOffset = DIMENSION / 2.0f * m_chunkDiameter - m_chunkDiameter * 0.5f;
	float x = -chunkOffset;
	float z = -chunkOffset;

	for( auto& chunk : m_chunks )
	{
		GGChunk::GGVoxelArray voxels = CreateRandomVoxels();

		XMFLOAT3 position = { x, 0.0f, z };
		x += m_chunkDiameter;
		if( x > chunkOffset )
		{
			x = -chunkOffset;
			z += m_chunkDiameter;
		}

		chunk.reset( new GGChunk( std::move( voxels ), position ) );
	}

	return;
}

GGChunk::GGVoxelArray GGWorld::CreateRandomVoxels()
{
	static default_random_engine gen;
	static bernoulli_distribution solid( 0.1 );

	GGChunk::GGVoxelArray voxels;
	for( auto& voxel : voxels )
	{
		if( solid( gen ) )
		{
			voxel.element = 1;
		}
	}

	return voxels;
}