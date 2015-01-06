#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <atlbase.h>

class GGMesh
{
public:
	struct GGBasicVertex
	{
		DirectX::XMFLOAT3 outputpos;
	};

public:
	GGMesh( UINT _indexCount, ID3D11Buffer* _vertexBuffer, ID3D11Buffer* _indexBuffer );

public:
	UINT GetIndexCount() const;
	ID3D11Buffer* GetVertexBuffer() const;
	ID3D11Buffer* GetIndexBuffer() const;

private:
	UINT m_indexCount;
	CComPtr<ID3D11Buffer> m_vertexBuffer;
	CComPtr<ID3D11Buffer> m_indexBuffer;
};