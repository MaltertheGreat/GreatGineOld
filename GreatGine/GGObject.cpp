#include "PCH.h"
#include "GGObject.h"
using namespace DirectX;
using namespace std;

GGObject::GGObject( float _voxelDimension )
	:
	m_voxelDimension( _voxelDimension ),
	m_voxels( { 0 } )
{}

GGObject::GGObject( GGVoxelArray && _voxels, float _voxelDimension, const XMFLOAT3 & _position )
	:
	m_voxelDimension( _voxelDimension ),
	m_position( _position ),
	m_voxels( _voxels )
{}

const XMFLOAT3& GGObject::GetPosition() const
{
	return m_position;
}

const GGObject::GGVoxelArray& GGObject::GetVoxels() const
{
	return m_voxels;
}