#pragma once

#include <d3d11.h>
#include <atlbase.h>

class GGWindow;

class GGDirectXDriver
{
public:
	GGDirectXDriver( const GGWindow& _window, UINT _resX, UINT _resY );

public:
	UINT GetResX();
	UINT GetResY();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();

private:
	UINT m_resX;
	UINT m_resY;

	CComPtr<ID3D11Device> m_device;
	CComPtr<ID3D11DeviceContext> m_deviceContext;
	CComPtr<IDXGISwapChain> m_swapChain;
};