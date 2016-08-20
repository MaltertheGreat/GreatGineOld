#include "PCH.h"
#include "GGObjectModel.h"
#include "GGDevice.h"
using namespace DirectX;
using namespace std;

GGObjectModel::GGObjectModel()
{}

GGObjectModel::GGObjectModel( const GGDevice& _device, const GGObject& _object, const XMFLOAT3& _position )
{
	m_mesh = _device.CreateMesh( CreateObjectGrid( _object ) );
	Update( _object, _position );
}

void GGObjectModel::Update( const GGObject& _object, const XMFLOAT3& _position )
{
	XMVECTOR position = XMLoadFloat3( &_position );
	position += XMLoadFloat3( &_object.GetPosition() );

	XMUINT3 objectDiameter = _object.GetDiameter();
	//float biggestDiameter = static_cast<float>(max( objectDiameter.x, max( objectDiameter.y, objectDiameter.z ) ));

	XMFLOAT3 objectScale =
	{
		_object.GetVoxelDimension() * objectDiameter.x,
		_object.GetVoxelDimension() * objectDiameter.y,
		_object.GetVoxelDimension() * objectDiameter.z,
	};
	XMMATRIX transformation = XMMatrixScaling( objectScale.x, objectScale.y, objectScale.z );
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

bool GGObjectModel::IsEmpty() const
{
	if( m_mesh.GetIndexCount() == 0 )
	{
		return true;
	}
	else
	{
		return false;
	}
}

const GGMesh GGObjectModel::GetMesh() const
{
	return m_mesh;
}

const XMFLOAT4X4& GGObjectModel::GetTransformation() const
{
	return m_transformation;
}

GGMeshData GGObjectModel::CreateObjectGrid( const GGObject& _object )
{
	GGMeshData grid;

	XMUINT3 objectDiameter = _object.GetDiameter();

	for( UINT x = 0; x < objectDiameter.x; ++x )
	{
		for( UINT y = 0; y < objectDiameter.y; ++y )
		{
			for( UINT z = 0; z < objectDiameter.z; ++z )
			{
				CreateVoxel( x, y, z, _object, grid );
			}
		}
	}

	return grid;
}

void GGObjectModel::CreateVoxel( UINT _x, UINT _y, UINT _z, const GGObject& _object, GGMeshData& _grid )
{
	const GGObject::GGVoxels& voxels = _object.GetVoxels();

	XMUINT3 objectDiameter = _object.GetDiameter();
	//	float biggestDiameter = static_cast<float>(max( objectDiameter.x, max( objectDiameter.y, objectDiameter.z ) ));

		//const float diameter = 1.0f / biggestDiameter;
	const XMFLOAT3 diameter =
	{
		1.0f / objectDiameter.x,
		1.0f / objectDiameter.y,
		1.0f / objectDiameter.z,
	};
	//const float radius = diameter / 2.0f;
	const XMFLOAT3 radius =
	{
		diameter.x / 2.0f,
		diameter.y / 2.0f,
		diameter.z / 2.0f
	};
	//const float offset = -radius * static_cast<float>(biggestDiameter - 1);
	const XMFLOAT3 offset =
	{
		-radius.x * static_cast<float>(objectDiameter.x - 1),
		-radius.y * static_cast<float>(objectDiameter.y - 1),
		-radius.z * static_cast<float>(objectDiameter.z - 1),
	};

	UINT voxelIndex = _x * objectDiameter.y * objectDiameter.z + _y * objectDiameter.z + _z;
	if( voxels[voxelIndex].element == 0 )
	{
		return;
	}

	XMFLOAT3 center = { offset.x + (_x*diameter.x), offset.y + (_y*diameter.y), offset.z + (_z*diameter.z) };
	XMFLOAT3 color = _object.GetColor();

	// Right
	if( (_x < (objectDiameter.x - 1u) && (voxels[voxelIndex + (objectDiameter.y * objectDiameter.z)].element == 0)) || (_x == (objectDiameter.x - 1u)) )
	{
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x + radius.x, center.y - radius.y, center.z - radius.z }, { 1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y + radius.y, center.z - radius.z }, { 1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y + radius.y, center.z + radius.z }, { 1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y - radius.y, center.z + radius.z }, { 1.0f, 0.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Top
	if( (_y < (objectDiameter.y - 1u) && (voxels[voxelIndex + (objectDiameter.z)].element == 0)) || (_y == (objectDiameter.y - 1u)) )
	{
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius.x, center.y + radius.y, center.z - radius.z }, { 0.0f, 1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y + radius.y, center.z + radius.z }, { 0.0f, 1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y + radius.y, center.z + radius.z }, { 0.0f, 1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y + radius.y, center.z - radius.z }, { 0.0f, 1.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Back
	if( (_z < (objectDiameter.z - 1u) && (voxels[voxelIndex + 1].element == 0)) || (_z == (objectDiameter.z - 1u)) )
	{
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x + radius.x, center.y - radius.y, center.z + radius.z }, { 0.0f, 0.0f, 1.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y + radius.y, center.z + radius.z }, { 0.0f, 0.0f, 1.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y + radius.y, center.z + radius.z }, { 0.0f, 0.0f, 1.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y - radius.y, center.z + radius.z }, { 0.0f, 0.0f, 1.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Left
	if( (_x > 0 && (voxels[voxelIndex - (objectDiameter.y * objectDiameter.z)].element == 0)) || (_x == 0) )
	{
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius.x, center.y - radius.y, center.z + radius.z }, { -1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y + radius.y, center.z + radius.z }, { -1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y + radius.y, center.z - radius.z }, { -1.0f, 0.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y - radius.y, center.z - radius.z }, { -1.0f, 0.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Bottom
	if( (_y > 0 && (voxels[voxelIndex - objectDiameter.z].element == 0)) || (_y == 0) )
	{
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius.x, center.y - radius.y, center.z + radius.z }, { 0.0f, -1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y - radius.y, center.z - radius.z }, { 0.0f, -1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y - radius.y, center.z - radius.z }, { 0.0f, -1.0f, 0.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y - radius.y, center.z + radius.z }, { 0.0f, -1.0f, 0.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	// Front
	if( (_z > 0 && (voxels[voxelIndex - 1].element == 0)) || (_z == 0) )
	{
		UINT indexCount = static_cast<UINT>(_grid.vertices.size());
		_grid.vertices.push_back( { { center.x - radius.x, center.y - radius.y, center.z - radius.z }, { 0.0f, 0.0f, -1.0f }, color } );
		_grid.vertices.push_back( { { center.x - radius.x, center.y + radius.y, center.z - radius.z }, { 0.0f, 0.0f, -1.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y + radius.y, center.z - radius.z }, { 0.0f, 0.0f, -1.0f }, color } );
		_grid.vertices.push_back( { { center.x + radius.x, center.y - radius.y, center.z - radius.z }, { 0.0f, 0.0f, -1.0f }, color } );

		_grid.indices.push_back( indexCount + 0 );
		_grid.indices.push_back( indexCount + 1 );
		_grid.indices.push_back( indexCount + 2 );

		_grid.indices.push_back( indexCount + 2 );
		_grid.indices.push_back( indexCount + 3 );
		_grid.indices.push_back( indexCount + 0 );
	}

	return;
}