#include "PCH.h"
#include "GGObject.h"
using namespace DirectX;
using namespace std;

GGObject::GGObject()
	:
	m_voxelDimension( 0.0f )
{}

GGObject::GGObject( GGVoxels&& _voxels, float _voxelDimension, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _color )
	:
	m_voxels( move( _voxels ) ),
	m_voxelDimension( _voxelDimension ),
	m_position( _position ),
	m_color( _color )
{
	bool empty = true;
	for( auto voxel : m_voxels )
	{
		if( voxel.element != 0 )
		{
			empty = false;
			break;
		}
	}

	if( empty )
	{
		m_voxels.clear();
		m_voxelDimension = 0.0f;
	}
}

void GGObject::SetPosition( const XMFLOAT3& _pos )
{
	m_position = _pos;
}

void GGObject::SetColor( const DirectX::XMFLOAT3& _color )
{
	m_color = _color;

	return;
}

const bool GGObject::IsEmpty() const
{
	if( m_voxels.empty() )
	{
		return true;
	}
	else
	{
		return false;
	}
}

const float GGObject::GetVoxelDimension() const
{
	return m_voxelDimension;
}

const XMFLOAT3& GGObject::GetPosition() const
{
	return m_position;
}

const XMFLOAT3& GGObject::GetColor() const
{
	return m_color;
}

const GGObject::GGVoxels& GGObject::GetVoxels() const
{
	return m_voxels;
}