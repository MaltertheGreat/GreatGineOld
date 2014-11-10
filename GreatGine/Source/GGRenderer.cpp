#include "GGRenderer.h"
#include "GGError.h"

GGRenderer::GGRenderer( const GGWindow& _window, UINT _resX, UINT _resY )
{
	if( !_resX && !_resY )
	{
		_resX = _window.GetWidth();
		_resY = _window.GetHeight();
	}

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

	CComPtr<ID3D11Texture2D> backBuffer;
	hr = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer) );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = m_device->CreateRenderTargetView( backBuffer, nullptr, &m_renderTargetView );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	m_deviceContext->OMSetRenderTargets( 1, &m_renderTargetView.p, nullptr );

	// Setup the viewport 
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT)_resX;
	vp.Height = (FLOAT)_resY;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports( 1, &vp );
}

void GGRenderer::ClearScene()
{
	float color[ 4 ] = { 0.2f, 0.2f, 0.8f, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_renderTargetView, color );

	return;
}

void GGRenderer::PresentScene()
{
	m_swapChain->Present( 0, 0 );

	return;
}