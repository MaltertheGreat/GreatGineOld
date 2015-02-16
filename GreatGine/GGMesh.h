#pragma once

#include "PCH.h"

class GGMesh
{
public:
	struct GGBasicVertex
	{
		DirectX::XMFLOAT3 outputpos;
	};

public:
	GGMesh( UINT _indexCount, Microsoft::WRL::ComPtr<ID3D11Buffer> _vertexBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> _indexBuffer );

public:
	UINT GetIndexCount() const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetVertexBuffer() const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetIndexBuffer() const;

private:
	UINT m_indexCount;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_indexBuffer;
};