#pragma once

#include <d3d11.h>
#include <atlbase.h>

#include "GGWindow.h"

class GGDirectXDevice
{
public:
	GGDirectXDevice( const GGWindow& _window, UINT _resX = 0, UINT _resY = 0 );

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	IDXGISwapChain* GetSwapChain() const;

	UINT GetResolutionX() const;
	UINT GetResolutionY() const;

private:
	CComPtr<ID3D11Device> m_device;
	CComPtr<ID3D11DeviceContext> m_deviceContext;
	CComPtr<IDXGISwapChain> m_swapChain;

	UINT m_resX;
	UINT m_resY;
};