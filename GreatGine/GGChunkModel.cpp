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
	// Work in progress

	float voxelRadius = 4 / static_cast<float>(_count);

	auto& depthLevel = _depthLevels[ _count - 1 ];
	for( auto& subdivisionLevel : depthLevel.subdivisionLevels )
	{
		auto& subdivisions = subdivisionLevel.subdivisions;
		
		// Left, down, front
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x -= voxelRadius;
			newCenter.y -= voxelRadius;
			newCenter.z -= voxelRadius;

			if( subdivisions[ 0 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}

		// Left, down, back
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x -= voxelRadius;
			newCenter.y -= voxelRadius;
			newCenter.z += voxelRadius;

			if( subdivisions[ 1 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}

		// Left, up, front
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x -= voxelRadius;
			newCenter.y += voxelRadius;
			newCenter.z -= voxelRadius;

			if( subdivisions[ 2 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}

		// Left, up, back
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x -= voxelRadius;
			newCenter.y += voxelRadius;
			newCenter.z += voxelRadius;

			if( subdivisions[ 3 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}

		// Right, down, front
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x += voxelRadius;
			newCenter.y -= voxelRadius;
			newCenter.z -= voxelRadius;

			if( subdivisions[ 4 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}

		// Right, down, back
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x += voxelRadius;
			newCenter.y -= voxelRadius;
			newCenter.z += voxelRadius;

			if( subdivisions[ 5 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}

		// Right, up, back
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x += voxelRadius;
			newCenter.y += voxelRadius;
			newCenter.z -= voxelRadius;

			if( subdivisions[ 6 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}

		// Right, up, front
		{
			XMFLOAT3 newCenter = _center;
			newCenter.x += voxelRadius;
			newCenter.y += voxelRadius;
			newCenter.z += voxelRadius;

			if( subdivisions[ 7 ] )
			{
				CreateMeshData( _meshData, _depthLevels, _lod, _count + 1, newCenter );
			}
			else
			{
				CreateCube( _meshData, newCenter, voxelRadius );
			}
		}
	}
}

void GGChunkModel::CreateCube( GGMeshData& _meshData, const DirectX::XMFLOAT3& _center, float _radius )
{
	GGMeshData::GGIndex lastIndex = _meshData.vertices.size();

	_meshData.vertices.push_back( { XMFLOAT3( _center.x - _radius, _center.y + _radius, _center.z - _radius ) } );
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
	_meshData.indices.push_back( lastIndex + 6 );

	return;
}