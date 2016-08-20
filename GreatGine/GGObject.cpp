#include "PCH.h"
#include "GGObject.h"
#include <cassert>
using namespace DirectX;
using namespace std;

GGObject::GGObject( GGVoxels && _voxels, float _voxelDimension, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _color, const DirectX::XMUINT3& _diameter )
	:
	m_voxels( move( _voxels ) ),
	m_voxelDimension( _voxelDimension ),
	m_position( _position ),
	m_color( _color )
{
	assert( m_voxels.size() == (_diameter.x * _diameter.y * _diameter.z) );

	m_diameter.x = static_cast<UCHAR>(_diameter.x);
	m_diameter.y = static_cast<UCHAR>(_diameter.y);
	m_diameter.z = static_cast<UCHAR>(_diameter.z);
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

const XMUINT3 GGObject::GetDiameter() const
{
	return{ m_diameter.x, m_diameter.y, m_diameter.z };
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