#include "PCH.h"
#include "GGChunkModel.h"
#include "GGDevice.h"
using namespace DirectX;
using namespace std;

void GGChunkModel::Create( const GGDevice& _device, const GGChunk& _chunk, UINT _lod )
{
	m_mesh = _device.CreateMesh( GGCubeMeshData( 16.0f ) );
	/*auto& depthLevels = _chunk.GetDepthLevels();
	if( _lod == 0 || _lod > depthLevels.size() )
	{
		_lod = depthLevels.size();
	}*/

	//GGMeshData meshData;

	// Set chunk model transformation matrix
	XMFLOAT3 position = _chunk.GetPosition();
	XMStoreFloat4x4( &m_transformation, XMMatrixTranslation( position.x, position.y, position.z ) );
}

const GGMesh* GGChunkModel::GetMesh() const
{
	return m_mesh.get();
}

const DirectX::XMFLOAT4X4& GGChunkModel::GetTransformation() const
{
	return m_transformation;
}

void GGChunkModel::CreateMeshData( GGMeshData& _meshData, const vector<unique_ptr<GGDepthLevel>>& _depthLevels, UINT _lod, UINT _count, const XMFLOAT3& _center )
{
	// Work in progress
	if( _count >= _lod )
	{
		return;
	}

	UINT voxelCount = _depthLevels[ _count ]->voxels.size();

	for( UINT i = 0; i < voxelCount; ++i )
	{
		auto subdivision = _depthLevels[ _count ]->subdivisions[ i ];
		if( subdivision )
		{
			XMFLOAT3 newCenter;
			CreateMeshData( _meshData, _depthLevels, _lod, _count + 1 );
		}
		else
		{

		}
	}
}