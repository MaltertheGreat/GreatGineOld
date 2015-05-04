#include "PCH.h"
#include "GGChunkModel.h"
#include "GGDevice.h"
using namespace DirectX;
using namespace std;

void GGChunkModel::Create( const GGDevice& _device, const GGChunk& _chunk )
{
	GGMeshData grid = CreateChunkGrid( _chunk );
	m_mesh = _device.CreateMesh( grid );

	// Set chunk model transformation matrix
	XMFLOAT3 position = _chunk.GetPosition();
	XMMATRIX trasformation = XMMatrixScaling( 16.0f, 16.0f, 16.0f );
	trasformation = XMMatrixMultiply( trasformation, XMMatrixTranslation( position.x, position.y, position.z ) );
	XMStoreFloat4x4( &m_transformation, trasformation );
}

const GGMesh* GGChunkModel::GetMesh() const
{
	return m_mesh.get();
}

const DirectX::XMFLOAT4X4& GGChunkModel::GetTransformation() const
{
	return m_transformation;
}

GGMeshData GGChunkModel::CreateChunkGrid( const GGChunk& _chunk )
{
	GGMeshData grid;

	auto& voxels = _chunk.GetVoxels();

	for( UINT x = 0; x < GGChunk::DIMENSION; ++x )
	{
		for( UINT y = 0; y < GGChunk::DIMENSION; ++y )
		{
			for( UINT z = 0; z < GGChunk::DIMENSION; ++z )
			{
				CreateVoxel( x, y, z, voxels, grid );
			}
		}
	}

	return grid;
}

void GGChunkModel::CreateVoxel( UINT x, UINT y, UINT z, const GGChunk::GGVoxelArray& voxels, GGMeshData& _grid )
{
	const float diameter = 1.0f / GGChunk::DIMENSION;
	const float radius = diameter / 2.0f;
	const float offset = radius * static_cast<float>(GGChunk::DIMENSION - 1);

	UINT voxelIndex = x * GGChunk::DIMENSION * GGChunk::DIMENSION + y * GGChunk::DIMENSION + z;
	if( voxels[ voxelIndex ].element == 0 )
	{
		return;
	}

	XMFLOAT3 center = { offset + (x*diameter), offset + (y*diameter), offset + (z*diameter) };
	XMFLOAT3 color = { 1.0f, 0.0f, 0.0f };

	// Right
	if( (x < (GGChunk::DIMENSION - 1) && (voxels[ voxelIndex + (GGChunk::DIMENSION * GGChunk::DIMENSION) ].element == 0)) || (x == (GGChunk::DIMENSION - 1)) )
	{
		UINT indexCount = _grid.vertices.size();
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z - radius }, { 1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z - radius }, { 1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z + radius }, { 1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z + radius }, { 1.0f, 0.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Top
	if( (y < (GGChunk::DIMENSION - 1) && (voxels[ voxelIndex + (GGChunk::DIMENSION) ].element == 0)) || (y == (GGChunk::DIMENSION - 1)) )
	{
		UINT indexCount = _grid.vertices.size();
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z - radius }, { 0.0f, 1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z + radius }, { 0.0f, 1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z + radius }, { 0.0f, 1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z - radius }, { 0.0f, 1.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Back
	if( (z < (GGChunk::DIMENSION - 1) && (voxels[ voxelIndex + 1 ].element == 0)) || (z == (GGChunk::DIMENSION - 1)) )
	{
		UINT indexCount = _grid.vertices.size();
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Left
	if( (x > 0 && (voxels[ voxelIndex - (GGChunk::DIMENSION * GGChunk::DIMENSION) ].element == 0)) || (x == 0) )
	{
		UINT indexCount = _grid.vertices.size();
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z + radius }, { -1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z + radius }, { -1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z - radius }, { -1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z - radius }, { -1.0f, 0.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Bottom
	if( (y > 0 && (voxels[ voxelIndex - GGChunk::DIMENSION ].element == 0)) || (y == 0) )
	{
		UINT indexCount = _grid.vertices.size();
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z + radius }, { 0.0f, -1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z - radius }, { 0.0f, -1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z - radius }, { 0.0f, -1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z + radius }, { 0.0f, -1.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Front
	if( (z > 0 && (voxels[ voxelIndex - 1 ].element == 0)) || (z == 0) )
	{
		UINT indexCount = _grid.vertices.size();
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}
	return;
}