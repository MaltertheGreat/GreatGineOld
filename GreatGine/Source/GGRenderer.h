#pragma once

#include <d3d11.h>
#include <atlbase.h>

#include "GGWindow.h"

class GGRenderer
{
public:
	GGRenderer( const GGWindow& _window, UINT _resX = 0, UINT _resY = 0 );

	void ClearScene();
	void PresentScene();

private:
	CComPtr<ID3D11Device> m_device;
	CComPtr<ID3D11DeviceContext> m_deviceContext;
	CComPtr<IDXGISwapChain> m_swapChain;

	CComPtr<ID3D11RenderTargetView> m_renderTargetView;
};