#pragma once

#include <d3d11_1.h>
#include <atlbase.h>
using namespace ATL;

#include "GGWindow.h"

class GGRenderer
{
public:
	GGRenderer( const GGWindow& _window, UINT _resX, UINT _resY );

	void ClearScene( const float _color[ 4 ] );
	void PresentScene();

private:
	void CreateDevice( const GGWindow& _window, UINT _resX, UINT _resY );
	void SetupOM();
	void SetupRS( UINT _resX, UINT _resY );

private:
	CComPtr<ID3D11Device> m_device;
	CComPtr<ID3D11DeviceContext> m_deviceContext;
	CComPtr<IDXGISwapChain> m_swapChain;

	CComPtr<ID3D11RenderTargetView> m_renderTargetView;
};