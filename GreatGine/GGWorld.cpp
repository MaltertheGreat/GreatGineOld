#include "PCH.h"
#include "GGWorld.h"
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

array<GGChunk, GGWorld::dimension * GGWorld::dimension>& GGWorld::GetChunkArray()
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
		GGSubdivisionLevel level = { { 1, 1, 1, 1, 1, 1, 1, 1 }, { 1 } };
		vector<GGSubdivisionLevel> subdivisionLevels = { level };

		vector<GGDepthLevel> depthLevels = { { subdivisionLevels } };

		// Testing code for debug purposes ;)
		// 2. depth level
		subdivisionLevels = { level };
		depthLevels.push_back( { subdivisionLevels } );

		// 3. depth level
		level.subdivisions.reset();
		subdivisionLevels = { level };
		depthLevels.push_back( { subdivisionLevels } );


		chunk.SetContent( move( depthLevels ) );

		XMFLOAT3 position = { x, 0.0f, z };
		chunk.SetPosition( position );

		x += m_chunkDiameter;
		if( x > chunkOffset )
		{
			x = -chunkOffset;
			z += m_chunkDiameter;
		}

		/*vector<unique_ptr<GGDepthLevel>> depthLevels;

		vector<GGVoxel> voxels = { 1 };
		vector<bool> subdivisions( voxels.size(), true );
		depthLevels.emplace_back( new GGDepthLevel( move( voxels ), move( subdivisions ) ) );

		RandomlyPopulateDepthLevel( depthLevels, subdivisions );
		RandomlyPopulateDepthLevel( depthLevels, depthLevels.back()->subdivisions );

		chunk.SetContent( move( depthLevels ) );*/
	}
	
	return;
}

void GGWorld::RandomlyPopulateDepthLevel( vector<unique_ptr<GGDepthLevel>>& _depthLevels, vector<bool>& _subdivisions )
{
	/*static const UINT subvoxelsInVoxel = 8;
	vector<GGVoxel> voxels;
	vector<bool> subdivisions;

	vector<GGVoxel> voxelsTemp( subvoxelsInVoxel, 1 );
	vector<bool> subdivisionsTemp( subvoxelsInVoxel, false );


	for( auto subdivision : _subdivisions )	// Every spot marked for subdivision gets 8 voxels
	{
		if( subdivision )
		{
			for( auto voxel : voxelsTemp )
			{
				voxels.push_back( voxel );
			}
			for( auto subdivision : subdivisionsTemp )
			{
				subdivisions.push_back( subdivision );
			}
		}
	}

	_depthLevels.emplace_back( new GGDepthLevel( move( voxels ), move( subdivisions ) ) );*/

	return;
}