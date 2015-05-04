#include "PCH.h"
#include "GGChunk.h"
using namespace DirectX;
using namespace std;

GGChunk::GGChunk()
	:
	m_hasChanged( true ),
	m_voxels( { { 0 } } )
{}

GGChunk::GGChunk( GGVoxelArray&& _voxels, const XMFLOAT3& _position )
	:
	m_hasChanged( true ),
	m_position( _position ),
	m_voxels( _voxels )
{}

void GGChunk::SetChangeState( bool _state )
{
	m_hasChanged = _state;

	return;
}

const XMFLOAT3& GGChunk::GetPosition() const
{
	return m_position;
}

bool GGChunk::HasChanged() const
{
	return m_hasChanged;
}

const GGChunk::GGVoxelArray& GGChunk::GetVoxels() const
{
	return m_voxels;
}