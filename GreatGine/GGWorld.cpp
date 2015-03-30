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

array<GGChunk, GGWorld::dimension * GGWorld::dimension>& GGWorld::GetChunkArray()
{
	return m_chunks;
}

void GGWorld::CreateWorld()
{
	float chunkOffset = dimension / 2.0f * m_chunkDiameter - m_chunkDiameter * 0.5f;
	float x = -chunkOffset;
	float z = -chunkOffset;

	for( auto& chunk : m_chunks )
	{
		/*GGSubdivisionLevel level = { { 1, 1, 0, 1, 1, 1, 1, 1 }, {} };
		level.subdivisions[ 2 ] = 1;
		vector<GGSubdivisionLevel> subdivisionLevels = { level };

		vector<GGDepthLevel> depthLevels = { { subdivisionLevels } };

		// Testing code for debug purposes ;)
		// 2. depth level
		level.voxels = { { 0, 0, 0, 0, 0, 1, 0, 0 } };
		level.subdivisions.reset();
		level.subdivisions[ 1 ] = 1;
		level.subdivisions[ 4 ] = 1;
		level.subdivisions[ 7 ] = 1;
		subdivisionLevels = { level };
		depthLevels.push_back( { subdivisionLevels } );

		// 3. depth level
		level.subdivisions.reset();
		level.voxels = { { 0, 0, 0, 0, 0, 1, 0, 0 } };
		subdivisionLevels = { level, level, level };
		depthLevels.push_back( { subdivisionLevels } );*/

		vector<std::unique_ptr<GGDepthLevel>> depthLevels;
		RandomlyPopulateChunk( depthLevels );

		chunk.SetContent( depthLevels );

		XMFLOAT3 position = { x, 0.0f, z };
		chunk.SetPosition( position );

		x += m_chunkDiameter;
		if( x > chunkOffset )
		{
			x = -chunkOffset;
			z += m_chunkDiameter;
		}
	}

	return;
}

bool GGWorld::RandomlyPopulateChunk( vector<std::unique_ptr<GGDepthLevel>>& _depthLevels, UINT _maxDepth, UINT _depth )
{
	static default_random_engine gen;
	static bernoulli_distribution subdivide( 0.25 );
	static bernoulli_distribution solid( 0.2 );

	int solidVoxelCount = 0;

	if( _depthLevels.size() <= _depth )
	{
		_depthLevels.push_back( {} );
	}

	GGSubdivisionLevel subdivisionLevel;
	auto& voxels = subdivisionLevel.voxels;
	auto& subdivisions = subdivisionLevel.subdivisions;
	for( UINT i = 0; i < subdivisions.size(); ++i )
	{
		if( subdivide( gen ) && (_depth + 1 < _maxDepth) )
		{
			if( RandomlyPopulateChunk( _depthLevels, _maxDepth, _depth + 1 ) )
			{
				voxels[ i ].element = _depth + 1;
			}
			subdivisions[ i ] = true;
		}
		else
		{
			if( solid( gen ) )
			{
				voxels[ i ].element = _depth + 1;
				++solidVoxelCount;
			}
		}
	}

	auto& depthLevel = _depthLevels[ _depth ];
	if( _depthLevels[ _depth ] )
	{
		depthLevel->subdivisionLevels.push_back( subdivisionLevel );
	}
	else
	{
		depthLevel.reset( new GGDepthLevel{ { subdivisionLevel } } );
	}

	return (solidVoxelCount >= 4);
}