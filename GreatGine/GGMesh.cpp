#include "PCH.h"
#include "GGMesh.h"
#include "GGError.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;

GGMesh::GGMesh( UINT _indexCount, ComPtr<ID3D11Buffer> _vertexBuffer, ComPtr<ID3D11Buffer> _indexBuffer )
	:
	m_indexCount( _indexCount ),
	m_vertexBuffer( _vertexBuffer ),
	m_indexBuffer( _indexBuffer )
{}

UINT GGMesh::GetIndexCount() const
{
	return m_indexCount;
}

ComPtr<ID3D11Buffer> GGMesh::GetVertexBuffer() const
{
	return m_vertexBuffer;
}

ComPtr<ID3D11Buffer> GGMesh::GetIndexBuffer() const
{
	return m_indexBuffer;
}