#pragma once

#include "PCH.h"

#include "GGCamera.h"
#include "GGShader.h"
#include "GGMesh.h"

class GGDirectXDriver;
struct GGMeshData;
struct GGLinesData;

class GGDevice
{
public:
	GGDevice( GGDirectXDriver& _driver );

public:
	GGCamera CreateCamera( float _fovAngle, UINT _viewWidth, UINT _viewHeight ) const;
	GGShader CreateShader() const;
	GGShader CreateLinesShader() const;
	GGMesh CreateMesh( const GGMeshData& _grid ) const;
	GGMesh CraeteLinesMesh( const GGLinesData& _lines ) const;

	void UpdateCamera( GGCamera& _camera, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation ) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
};