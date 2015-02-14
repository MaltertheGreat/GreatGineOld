#pragma once

#include <d3d11.h>
#include <d2d1.h>
#include <atlbase.h>

class GGWindow;

class GGDirectXDriver
{
public:
	GGDirectXDriver( const GGWindow& _window, UINT _resX, UINT _resY );

public:
	UINT GetResX() const;
	UINT GetResY() const;

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;
	IDXGISwapChain* GetSwapChain() const;
	ID2D1Factory* GetFactory2D() const;

private:
	UINT m_resX;
	UINT m_resY;

	CComPtr<ID3D11Device> m_device;
	CComPtr<ID3D11DeviceContext> m_deviceContext;
	CComPtr<IDXGISwapChain> m_swapChain;
	CComPtr<ID2D1Factory> m_factory2d;
};