#include "GGMesh.h"
#include "GGError.h"
using namespace DirectX;

GGMesh::GGMesh( UINT _indexCount, ID3D11Buffer* _vertexBuffer, ID3D11Buffer* _indexBuffer )
	:
	m_indexCount( _indexCount ),
	m_vertexBuffer( _vertexBuffer ),
	m_indexBuffer( _indexBuffer )
{}

GGMesh::GGMesh( GGMesh&& _from )
{
	m_indexCount = _from.m_indexCount;
	m_vertexBuffer.Attach( _from.m_vertexBuffer.Detach() );
	m_indexBuffer.Attach( _from.m_indexBuffer.Detach() );
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