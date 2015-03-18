#pragma once

#include "PCH.h"

#include "GGCamera.h"
#include "GGShader.h"
#include "GGMesh.h"

class GGDirectXDriver;
class GGShader;
class GGMesh;

class GGDevice
{
public:
	GGDevice( GGDirectXDriver& _driver );

public:
	UINT GetResolutionX() const;
	UINT GetResolutionY() const;

	GGCamera CreateCamera( float _fovAngle, UINT _viewWidth, UINT _viewHeight );
	GGShader CreateShader();
	GGMesh CreateTriangleMesh();
	GGMesh CreateCubeMesh( float _diameter );

	void UpdateCamera( GGCamera& _camera, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation );

private:
	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
};