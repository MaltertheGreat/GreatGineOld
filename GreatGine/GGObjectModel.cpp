#include "PCH.h"
#include "GGObjectModel.h"
#include "GGDevice.h"
using namespace DirectX;
using namespace std;

GGObjectModel::GGObjectModel()
{}

GGObjectModel::GGObjectModel( const GGDevice& _device, const GGObject& _object, const XMFLOAT3& _position, const XMFLOAT3& _color )
	:
	m_mesh( _device.CreateMesh( CreateObjectGrid( _object, _color ) ) )
{
	// Set object model transformation matrix

	XMVECTOR position = XMLoadFloat3( &_position );
	position += XMLoadFloat3( &_object.GetPosition() );

	float objectScale = _object.GetVoxelDimension() * GGObject::DIAMETER;
	XMMATRIX trasformation = XMMatrixScaling( objectScale, objectScale, objectScale );
	trasformation = XMMatrixMultiply( trasformation, XMMatrixTranslationFromVector( position ) );
	XMStoreFloat4x4( &m_transformation, trasformation );
}

void GGObjectModel::Update( const GGObject& _object, const XMFLOAT3& _position )
{
	XMVECTOR position = XMLoadFloat3( &_position );
	position += XMLoadFloat3( &_object.GetPosition() );

	float objectScale = _object.GetVoxelDimension() * GGObject::DIAMETER;
	XMMATRIX transformation = XMMatrixScaling( objectScale, objectScale, objectScale );
	transformation = XMMatrixMultiply( transformation, XMMatrixTranslationFromVector( position ) );
	XMStoreFloat4x4( &m_transformation, transformation );
}

void GGObjectModel::Move( const XMFLOAT3& _offset )
{
	XMMATRIX transformation = XMLoadFloat4x4( &m_transformation );
	transformation = XMMatrixMultiply( transformation, XMMatrixTranslation( _offset.x, _offset.y, _offset.z ) );

	XMStoreFloat4x4( &m_transformation, transformation );

	return;
}

const GGMesh GGObjectModel::GetMesh() const
{
	return m_mesh;
}

const XMFLOAT4X4& GGObjectModel::GetTransformation() const
{
	return m_transformation;
}

GGMeshData GGObjectModel::CreateObjectGrid( const GGObject& _object, const XMFLOAT3& _color )
{
	GGMeshData grid;

	auto& voxels = _object.GetVoxels();

	for( UINT x = 0; x < GGObject::DIAMETER; ++x )
	{
		for( UINT y = 0; y < GGObject::DIAMETER; ++y )
		{
			for( UINT z = 0; z < GGObject::DIAMETER; ++z )
			{
				CreateVoxel( x, y, z, voxels, grid, _color );
			}
		}
	}

	return grid;
}

void GGObjectModel::CreateVoxel( UINT x, UINT y, UINT z, const GGObject::GGVoxelArray& voxels, GGMeshData& _grid, const XMFLOAT3& _color )
{
	const float diameter = 1.0f / GGObject::DIAMETER;
	const float radius = diameter / 2.0f;
	const float offset = -radius * static_cast<float>(GGObject::DIAMETER - 1);

	UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
	if( voxels[ voxelIndex ].element == 0 )
	{
		return;
	}

	XMFLOAT3 center = { offset + (x*diameter), offset + (y*diameter), offset + (z*diameter) };
	XMFLOAT3 color = { 0.2314f, 0.8196f, 0.0f };

	// Right
	if( (x < (GGObject::DIAMETER - 1) && (voxels[ voxelIndex + (GGObject::DIAMETER * GGObject::DIAMETER) ].element == 0)) || (x == (GGObject::DIAMETER - 1)) )
	{
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z - radius }, { 1.0f, 0.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z - radius }, { 1.0f, 0.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z + radius }, { 1.0f, 0.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z + radius }, { 1.0f, 0.0f, 0.0f }, _color } );

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
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z - radius }, { 0.0f, 1.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z + radius }, { 0.0f, 1.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z + radius }, { 0.0f, 1.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z - radius }, { 0.0f, 1.0f, 0.0f }, _color } );

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
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z + radius }, { 0.0f, 0.0f, 1.0f }, _color } );

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
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z + radius }, { -1.0f, 0.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z + radius }, { -1.0f, 0.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z - radius }, { -1.0f, 0.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z - radius }, { -1.0f, 0.0f, 0.0f }, _color } );

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
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z + radius }, { 0.0f, -1.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z - radius }, { 0.0f, -1.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z - radius }, { 0.0f, -1.0f, 0.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z + radius }, { 0.0f, -1.0f, 0.0f }, _color } );

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
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius, center.y - radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, _color } );
		_grid.vertices.push_back( { { center.x - radius, center.y + radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y + radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, _color } );
		_grid.vertices.push_back( { { center.x + radius, center.y - radius, center.z - radius }, { 0.0f, 0.0f, -1.0f }, _color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}
	return;
}