#pragma once

#include "GGDirectXDevice.h"

class GGRenderer
{
public:
	GGRenderer( const GGDirectXDevice& _device );

	void ClearScene();
	void PresentScene();

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;

	CComPtr<ID3D11RenderTargetView> m_renderTargetView;
};