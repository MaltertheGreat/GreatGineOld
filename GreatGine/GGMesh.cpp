#include "GGMesh.h"
#include "GGError.h"
using namespace DirectX;

GGMesh::GGMesh( UINT _indexCount, ID3D11Buffer* _vertexBuffer, ID3D11Buffer* _indexBuffer )
	:
	m_indexCount( _indexCount ),
	m_vertexBuffer( _vertexBuffer ),
	m_indexBuffer( _indexBuffer )
{
}

UINT GGMesh::GetIndexCount() const
{
	return m_indexCount;
}

ID3D11Buffer* GGMesh::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

ID3D11Buffer* GGMesh::GetIndexBuffer() const
{
	return m_indexBuffer;
}