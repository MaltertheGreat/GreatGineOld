#pragma once

#include <d3d11.h>

class GGDirectXDriver;
class GGBasicShader;

class GGDevice
{
public:
	GGDevice( GGDirectXDriver& _driver );

public:
	UINT GetResolutionX() const;
	UINT GetResolutionY() const;

	GGBasicShader* CreateBasicShader();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
};