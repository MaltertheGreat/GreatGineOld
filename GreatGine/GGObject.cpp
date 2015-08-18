#include "PCH.h"
#include "GGObject.h"
#include <random>
using namespace DirectX;
using namespace std;

GGObject::GGObject( GGVoxelArray&& _voxels, float _voxelDimension, const XMFLOAT3& _position )
	:
	m_voxelDimension( _voxelDimension ),
	m_position( _position ),
	m_voxels( move( _voxels ) )
{
	static random_device gen;
	static uniform_real_distribution<float> color( 0.25f, 0.75f );

	m_color.x = color( gen );
	m_color.y = color( gen );
	m_color.z = color( gen );
}

void GGObject::SetPosition( const XMFLOAT3 & _pos )
{
	m_position = _pos;
}

void GGObject::SetColor( const DirectX::XMFLOAT3 & _color )
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

const GGObject::GGVoxelArray& GGObject::GetVoxels() const
{
	return m_voxels;
}