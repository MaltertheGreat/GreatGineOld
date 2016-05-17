#include "PCH.h"
#include "GGObject.h"
using namespace DirectX;
using namespace std;

GGObject::GGObject( GGVoxels&& _voxels, float _voxelDimension, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _color )
	:
	m_voxels( move( _voxels ) ),
	m_voxelDimension( _voxelDimension ),
	m_position( _position ),
	m_color( _color )
{}

void GGObject::SetPosition( const XMFLOAT3& _pos )
{
	m_position = _pos;
}

void GGObject::SetColor( const DirectX::XMFLOAT3& _color )
{
	m_color = _color;

	return;
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