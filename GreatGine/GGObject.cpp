#include "PCH.h"
#include "GGObject.h"
using namespace DirectX;
using namespace std;

GGObject::GGObject( GGVoxelArray && _voxels, float _voxelDimension, const XMFLOAT3 & _position )
	:
	m_voxelDimension( _voxelDimension ),
	m_position( _position ),
	m_voxels( move( _voxels ) )
{}

const float GGObject::GetVoxelDimension() const
{
	return m_voxelDimension;
}

const XMFLOAT3& GGObject::GetPosition() const
{
	return m_position;
}

const GGObject::GGVoxelArray& GGObject::GetVoxels() const
{
	return m_voxels;
}

void GGObject::SetPosition( const DirectX::XMFLOAT3 & _pos )
{
	m_position = _pos;
}