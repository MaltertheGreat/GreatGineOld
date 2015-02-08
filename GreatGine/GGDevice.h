#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

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

	GGCamera CreateCamera( float _fovAngle );
	GGShader CreateShader();
	GGMesh CreateTriangleMesh();
	GGMesh CreateCubeMesh();

	void UpdateCamera( GGCamera& _camera, DirectX::XMFLOAT4X4& viewMatrix );

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
};