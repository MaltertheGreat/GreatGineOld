#include "GGRenderer.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include "GGError.h"

GGRenderer::GGRenderer( const GGWindow& _window, UINT _resX, UINT _resY )
{
	UINT resX;
	UINT resY;
	if( _resX && _resY )
	{
		resX = _resX;
		resY = _resY;
	}
	else
	{
		resX = _window.GetWidth();
		resY = _window.GetHeight();
	}

	CreateDevice( _window, resX, resY );
	SetupOM();
	SetupRS( resX, resY );
}

void GGRenderer::ClearScene( const float _color[ 4 ] )
{
	m_deviceContext->ClearRenderTargetView( m_renderTargetView, _color );

	return;
}

void GGRenderer::PresentScene()
{
	m_swapChain->Present( 0, 0 );

	return;
}

void GGRenderer::CreateDevice( const GGWindow & _window, UINT _resX, UINT _resY )
{
	UINT createDeviceFlags = 0;
	#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
	#endif

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0
	};
	UINT numFeatureLevels = ARRAYSIZE( featureLevels );

	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory( &sd, sizeof( sd ) );
	sd.BufferCount = 1;
	sd.BufferDesc.Width = _resX;
	sd.BufferDesc.Height = _resY;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = _window.GetHandle();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;

	HRESULT hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, &m_swapChain, &m_device, nullptr, &m_deviceContext );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return;
}

void GGRenderer::SetupOM()
{
	CComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer) );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = m_device->CreateRenderTargetView( backBuffer, nullptr, &m_renderTargetView );

	m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView.p, nullptr );

	return;
}

void GGRenderer::SetupRS( UINT _resX, UINT _resY )
{
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_resX;
	vp.Height = (FLOAT)_resY;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports( 1, &vp );

	return;
}
