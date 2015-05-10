#include "PCH.h"
#include "GGObjectModel.h"
#include "GGDevice.h"
using namespace DirectX;
using namespace std;

void GGObjectModel::Create( const GGDevice& _device, const GGObject& _object )
{
	GGMeshData grid = CreateObjectGrid( _object );
	m_mesh = _device.CreateMesh( grid );

	// Set object model transformation matrix
	XMFLOAT3 position = _object.GetPosition();
	XMMATRIX trasformation = XMMatrixScaling( 16.0f, 16.0f, 16.0f );
	trasformation = XMMatrixMultiply( trasformation, XMMatrixTranslation( position.x, position.y, position.z ) );
	XMStoreFloat4x4( &m_transformation, trasformation );
}

const GGMesh* GGObjectModel::GetMesh() const
{
	return m_mesh.get();
}

const DirectX::XMFLOAT4X4& GGObjectModel::GetTransformation() const
{
	return m_transformation;
}

GGMeshData GGObjectModel::CreateObjectGrid( const GGObject& _object )
{
	GGMeshData grid;

	auto& voxels = _object.GetVoxels();

	for( UINT x = 0; x < GGObject::DIAMETER; ++x )
	{
		for( UINT y = 0; y < GGObject::DIAMETER; ++y )
		{
			for( UINT z = 0; z < GGObject::DIAMETER; ++z )
			{
				CreateVoxel( x, y, z, voxels, grid );
			}
		}
	}

	return grid;
}

void GGObjectModel::CreateVoxel( UINT x, UINT y, UINT z, const GGObject::GGVoxelArray& voxels, GGMeshData& _grid )
{
	const float diameter = 1.0f / GGObject::DIAMETER;
	const float radius = diameter / 2.0f;
	const float offset = radius * static_cast<float>(GGObject::DIAMETER - 1);

	UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
	if( voxels[ voxelIndex ].element == 0 )
	{
		return;
	}

	XMFLOAT3 center = { offset + (x*diameter), offset + (y*diameter), offset + (z*diameter) };
	XMFLOAT3 color = { 1.0f, 0.0f, 0.0f };

	// Right
	if( (x < (GGObject::DIAMETER - 1) && (voxels[ voxelIndex + (GGObject::DIAMETER * GGObject::DIAMETER) ].element == 0)) || (x == (GGObject::DIAMETER - 1)) )
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
	if( (y < (GGObject::DIAMETER - 1) && (voxels[ voxelIndex + (GGObject::DIAMETER) ].element == 0)) || (y == (GGObject::DIAMETER - 1)) )
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
	if( (z < (GGObject::DIAMETER - 1) && (voxels[ voxelIndex + 1 ].element == 0)) || (z == (GGObject::DIAMETER - 1)) )
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
	if( (x > 0 && (voxels[ voxelIndex - (GGObject::DIAMETER * GGObject::DIAMETER) ].element == 0)) || (x == 0) )
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
	if( (y > 0 && (voxels[ voxelIndex - GGObject::DIAMETER ].element == 0)) || (y == 0) )
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