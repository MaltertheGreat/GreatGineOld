#pragma once

#include <d3d11.h>

class GGDirectXDriver;
class GGShader;
class GGMesh;

class GGDevice
{
public:
	GGDevice( GGDirectXDriver& _driver );
	~GGDevice();

public:
	UINT GetResolutionX() const;
	UINT GetResolutionY() const;

	GGShader* CreateShader();
	GGMesh* CreateTriangleMesh();
	GGMesh* CreateCubeMesh();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
};