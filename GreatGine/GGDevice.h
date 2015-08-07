#pragma once

#include "PCH.h"

#include "GGDirectXDriver.h"
#include "GGShader.h"
#include "GGMesh.h"
#include "GGMeshData.h"

class GGWindow;
struct GGMeshData;
struct GGLinesData;

class GGDevice
{
public:
	GGDevice( const GGWindow& _window, UINT _resX, UINT _resY );

public:
	GGDirectXDriver GetDriver();

	GGShader CreateShader() const;
	GGShader CreateLinesShader() const;
	GGMesh CreateMesh( const GGMeshData& _grid ) const;
	GGMesh CraeteLinesMesh( const GGLinesData& _lines ) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> CreateVertexBuffer( UINT _size, const void* _data ) const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> CreateIndexBuffer( UINT _size, const void* _data ) const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> CreateConstantBuffer( UINT _size, const void* _data ) const;

private:
	UINT m_resX;
	UINT m_resY;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID2D1Factory> m_factory2d;
};