#pragma once

#include "PCH.h"

#include "GGCamera.h"
#include "GGShader.h"
#include "GGMesh.h"

class GGDirectXDriver;
struct GGMeshData;

class GGDevice
{
public:
	GGDevice( GGDirectXDriver& _driver );

public:
	UINT GetResolutionX() const;
	UINT GetResolutionY() const;

	GGCamera CreateCamera( float _fovAngle, UINT _viewWidth, UINT _viewHeight ) const;
	GGShader CreateShader() const;
	std::unique_ptr<GGMesh> CreateMesh( const GGMeshData& _grid ) const;

	void UpdateCamera( GGCamera& _camera, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation ) const;

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
};