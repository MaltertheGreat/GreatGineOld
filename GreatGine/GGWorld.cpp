#include "PCH.h"
#include "GGWorld.h"
using namespace DirectX;
using namespace std;

const float GGWorld::m_chunkDiameter = 16.0f;

GGWorld::GGWorld( GGInputProcessor& _inputProcessor )
	:
	m_freeCamera( { 0.0f, 1.0f, -5.0f } )
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

const array<GGChunk, GGWorld::dimension * GGWorld::dimension>& GGWorld::GetChunkArray() const
{
	return m_chunks;
}

void GGWorld::CreateWorld()
{
	float chunkOffset = dimension / 2.0f * m_chunkDiameter - m_chunkDiameter * 0.5f;
	float x = -chunkOffset;
	float z = -chunkOffset;

	for( auto& chunk: m_chunks )
	{
		vector<unique_ptr<GGDepthLevel>> depthLevels;

		vector<GGVoxel> voxels = { 1 };
		vector<bool> subdivisions( voxels.size(), false );
		depthLevels.emplace_back( new GGDepthLevel( move( voxels ), move( subdivisions ) ) );

		chunk.SetContent( move( depthLevels ) );


		XMFLOAT4X4 chunkTransform;
		XMStoreFloat4x4( &chunkTransform, XMMatrixTranslation( x, 0.0f, z ) );
		chunk.SetTransformation( chunkTransform );
		
		x += m_chunkDiameter;
		if( x > chunkOffset )
		{
			x = -chunkOffset;
			z += m_chunkDiameter;
		}
	}
	
	return;
}