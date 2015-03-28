#include "PCH.h"
#include "GGChunkModel.h"
#include "GGDevice.h"
using namespace DirectX;
using namespace std;

void GGChunkModel::Create( const GGDevice& _device, const GGChunk& _chunk, UINT _lod )
{
	//m_mesh = _device.CreateMesh( GGCubeMeshData( 16.0f ) );
	auto& depthLevels = _chunk.GetDepthLevels();
	if( _lod == 0 || _lod > depthLevels.size() )
	{
		_lod = depthLevels.size();
	}

	GGMeshData meshData;
	CreateMeshData( meshData, depthLevels, _lod );
	m_mesh = _device.CreateMesh( meshData );

	// Set chunk model transformation matrix
	XMFLOAT3 position = _chunk.GetPosition();
	XMStoreFloat4x4( &m_transformation, XMMatrixTranslation( position.x, position.y, position.z ) );
}

const GGMesh* GGChunkModel::GetMesh() const
{
	return m_mesh.get();
}

const DirectX::XMFLOAT4X4& GGChunkModel::GetTransformation() const
{
	return m_transformation;
}

void GGChunkModel::CreateMeshData( GGMeshData& _meshData, const vector<GGDepthLevel>& _depthLevels, UINT _lod, UINT _count, const XMFLOAT3& _center )
{
	float chunkRadius = 4.0f;
	float depthLevelDimension = powf( 2, static_cast<float>(_count - 1) ); // Count in voxels
	float voxelRadius = chunkRadius / depthLevelDimension;
	array<XMFLOAT3, 8> newCenters = CreateCentersArray( _center, voxelRadius );

	auto& depthLevel = _depthLevels[ _count - 1 ];
	for( auto& subdivisionLevel : depthLevel.subdivisionLevels )
	{
		auto& subdivisions = subdivisionLevel.subdivisions;
		for( UINT i = 0; i < 8; ++i )
		{
			XMFLOAT3 newCenter = newCenters[ i ];
			if( subdivisions[ i ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				if( subdivisionLevel.voxels[ i ].element )
				{
					CreateVoxel( _meshData, newCenter, voxelRadius );
				}
			}
		}
	}
}

array<XMFLOAT3, 8> GGChunkModel::CreateCentersArray( const XMFLOAT3& _center, float _voxelRadius )
{
	array<XMFLOAT3, 8> newCenters;
	newCenters.fill( _center );

	// Left, down, front
	newCenters[ 0 ].x -= _voxelRadius;
	newCenters[ 0 ].y -= _voxelRadius;
	newCenters[ 0 ].z -= _voxelRadius;

	// Left, down, back
	newCenters[ 1 ].x -= _voxelRadius;
	newCenters[ 1 ].y -= _voxelRadius;
	newCenters[ 1 ].z += _voxelRadius;

	// Left, up, front
	newCenters[ 2 ].x -= _voxelRadius;
	newCenters[ 2 ].y += _voxelRadius;
	newCenters[ 2 ].z -= _voxelRadius;

	// Left, up, back
	newCenters[ 3 ].x -= _voxelRadius;
	newCenters[ 3 ].y += _voxelRadius;
	newCenters[ 3 ].z += _voxelRadius;

	// Right, down, front
	newCenters[ 4 ].x += _voxelRadius;
	newCenters[ 4 ].y -= _voxelRadius;
	newCenters[ 4 ].z -= _voxelRadius;

	// Right, down, back
	newCenters[ 5 ].x += _voxelRadius;
	newCenters[ 5 ].y -= _voxelRadius;
	newCenters[ 5 ].z += _voxelRadius;

	// Right, up, back
	newCenters[ 6 ].x += _voxelRadius;
	newCenters[ 6 ].y += _voxelRadius;
	newCenters[ 6 ].z -= _voxelRadius;

	// Right, up, front
	newCenters[ 7 ].x += _voxelRadius;
	newCenters[ 7 ].y += _voxelRadius;
	newCenters[ 7 ].z += _voxelRadius;

	return newCenters;
}

void GGChunkModel::CreateVoxel( GGMeshData& _meshData, const DirectX::XMFLOAT3& _center, float _radius )
{
	// Front
	{
		UINT indexCount = _meshData.vertices.size();
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y - _radius, _center.z - _radius }, { 0.0f, 0.0f, -1.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y + _radius, _center.z - _radius }, { 0.0f, 0.0f, -1.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y + _radius, _center.z - _radius }, { 0.0f, 0.0f, -1.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y - _radius, _center.z - _radius }, { 0.0f, 0.0f, -1.0f } } );

		_meshData.indices.push_back( indexCount + 0 );
		_meshData.indices.push_back( indexCount + 1 );
		_meshData.indices.push_back( indexCount + 2 );

		_meshData.indices.push_back( indexCount + 2 );
		_meshData.indices.push_back( indexCount + 3 );
		_meshData.indices.push_back( indexCount + 0 );
	}

	// Back
	{
		UINT indexCount = _meshData.vertices.size();
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y - _radius, _center.z + _radius }, { 0.0f, 0.0f, 1.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y + _radius, _center.z + _radius }, { 0.0f, 0.0f, 1.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y + _radius, _center.z + _radius }, { 0.0f, 0.0f, 1.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y - _radius, _center.z + _radius }, { 0.0f, 0.0f, 1.0f } } );

		_meshData.indices.push_back( indexCount + 0 );
		_meshData.indices.push_back( indexCount + 1 );
		_meshData.indices.push_back( indexCount + 2 );

		_meshData.indices.push_back( indexCount + 2 );
		_meshData.indices.push_back( indexCount + 3 );
		_meshData.indices.push_back( indexCount + 0 );
	}

	// Right
	{
		UINT indexCount = _meshData.vertices.size();
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y - _radius, _center.z - _radius }, { 1.0f, 0.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y + _radius, _center.z - _radius }, { 1.0f, 0.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y + _radius, _center.z + _radius }, { 1.0f, 0.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y - _radius, _center.z + _radius }, { 1.0f, 0.0f, 0.0f } } );

		_meshData.indices.push_back( indexCount + 0 );
		_meshData.indices.push_back( indexCount + 1 );
		_meshData.indices.push_back( indexCount + 2 );

		_meshData.indices.push_back( indexCount + 2 );
		_meshData.indices.push_back( indexCount + 3 );
		_meshData.indices.push_back( indexCount + 0 );
	}

	// Left
	{
		UINT indexCount = _meshData.vertices.size();
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y - _radius, _center.z + _radius }, { -1.0f, 0.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y + _radius, _center.z + _radius }, { -1.0f, 0.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y + _radius, _center.z - _radius }, { -1.0f, 0.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y - _radius, _center.z - _radius }, { -1.0f, 0.0f, 0.0f } } );

		_meshData.indices.push_back( indexCount + 0 );
		_meshData.indices.push_back( indexCount + 1 );
		_meshData.indices.push_back( indexCount + 2 );

		_meshData.indices.push_back( indexCount + 2 );
		_meshData.indices.push_back( indexCount + 3 );
		_meshData.indices.push_back( indexCount + 0 );
	}

	// Top
	{
		UINT indexCount = _meshData.vertices.size();
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y + _radius, _center.z - _radius }, { 0.0f, 1.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y + _radius, _center.z + _radius }, { 0.0f, 1.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y + _radius, _center.z + _radius }, { 0.0f, 1.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y + _radius, _center.z - _radius }, { 0.0f, 1.0f, 0.0f } } );

		_meshData.indices.push_back( indexCount + 0 );
		_meshData.indices.push_back( indexCount + 1 );
		_meshData.indices.push_back( indexCount + 2 );

		_meshData.indices.push_back( indexCount + 2 );
		_meshData.indices.push_back( indexCount + 3 );
		_meshData.indices.push_back( indexCount + 0 );
	}

	// Bottom
	{
		UINT indexCount = _meshData.vertices.size();
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y - _radius, _center.z + _radius }, { 0.0f, -1.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x - _radius, _center.y - _radius, _center.z - _radius }, { 0.0f, -1.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y - _radius, _center.z - _radius }, { 0.0f, -1.0f, 0.0f } } );
		_meshData.vertices.push_back( { { _center.x + _radius, _center.y - _radius, _center.z + _radius }, { 0.0f, -1.0f, 0.0f } } );

		_meshData.indices.push_back( indexCount + 0 );
		_meshData.indices.push_back( indexCount + 1 );
		_meshData.indices.push_back( indexCount + 2 );

		_meshData.indices.push_back( indexCount + 2 );
		_meshData.indices.push_back( indexCount + 3 );
		_meshData.indices.push_back( indexCount + 0 );
	}

	/*_meshData.vertices.push_back( { XMFLOAT3( _center.x - _radius, _center.y + _radius, _center.z - _radius ) } );
	_meshData.vertices.push_back( { XMFLOAT3( _center.x + _radius, _center.y + _radius, _center.z - _radius ) } );
	_meshData.vertices.push_back( { XMFLOAT3( _center.x + _radius, _center.y + _radius, _center.z + _radius ) } );
	_meshData.vertices.push_back( { XMFLOAT3( _center.x - _radius, _center.y + _radius, _center.z + _radius ) } );
	_meshData.vertices.push_back( { XMFLOAT3( _center.x - _radius, _center.y - _radius, _center.z - _radius ) } );
	_meshData.vertices.push_back( { XMFLOAT3( _center.x + _radius, _center.y - _radius, _center.z - _radius ) } );
	_meshData.vertices.push_back( { XMFLOAT3( _center.x + _radius, _center.y - _radius, _center.z + _radius ) } );
	_meshData.vertices.push_back( { XMFLOAT3( _center.x - _radius, _center.y - _radius, _center.z + _radius ) } );

	_meshData.indices.push_back( lastIndex + 3 );
	_meshData.indices.push_back( lastIndex + 1 );
	_meshData.indices.push_back( lastIndex + 0 );

	_meshData.indices.push_back( lastIndex + 2 );
	_meshData.indices.push_back( lastIndex + 1 );
	_meshData.indices.push_back( lastIndex + 3 );

	_meshData.indices.push_back( lastIndex + 0 );
	_meshData.indices.push_back( lastIndex + 5 );
	_meshData.indices.push_back( lastIndex + 4 );

	_meshData.indices.push_back( lastIndex + 1 );
	_meshData.indices.push_back( lastIndex + 5 );
	_meshData.indices.push_back( lastIndex + 0 );

	_meshData.indices.push_back( lastIndex + 3 );
	_meshData.indices.push_back( lastIndex + 4 );
	_meshData.indices.push_back( lastIndex + 7 );

	_meshData.indices.push_back( lastIndex + 0 );
	_meshData.indices.push_back( lastIndex + 4 );
	_meshData.indices.push_back( lastIndex + 3 );

	_meshData.indices.push_back( lastIndex + 1 );
	_meshData.indices.push_back( lastIndex + 6 );
	_meshData.indices.push_back( lastIndex + 5 );

	_meshData.indices.push_back( lastIndex + 2 );
	_meshData.indices.push_back( lastIndex + 6 );
	_meshData.indices.push_back( lastIndex + 1 );

	_meshData.indices.push_back( lastIndex + 2 );
	_meshData.indices.push_back( lastIndex + 7 );
	_meshData.indices.push_back( lastIndex + 6 );

	_meshData.indices.push_back( lastIndex + 3 );
	_meshData.indices.push_back( lastIndex + 7 );
	_meshData.indices.push_back( lastIndex + 2 );

	_meshData.indices.push_back( lastIndex + 6 );
	_meshData.indices.push_back( lastIndex + 4 );
	_meshData.indices.push_back( lastIndex + 5 );

	_meshData.indices.push_back( lastIndex + 7 );
	_meshData.indices.push_back( lastIndex + 4 );
	_meshData.indices.push_back( lastIndex + 6 );*/

	return;
}