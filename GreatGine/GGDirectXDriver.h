#pragma once

#include "PCH.h"

class GGWindow;

class GGDirectXDriver
{
public:
	GGDirectXDriver( const GGWindow& _window, UINT _resX, UINT _resY );

public:
	UINT GetResX() const;
	UINT GetResY() const;

	Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() const;
	Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const;
	Microsoft::WRL::ComPtr<ID2D1Factory> GetFactory2D() const;

private:
	UINT m_resX;
	UINT m_resY;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID2D1Factory> m_factory2d;
};