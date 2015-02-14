#include "GGDirectXDriver.h"
#include "GGWindow.h"
#include "GGError.h"

GGDirectXDriver::GGDirectXDriver( const GGWindow& _window, UINT _resX, UINT _resY )
	:
	m_resX( _resX ),
	m_resY( _resY )
{
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
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
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
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

	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_factory2d );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}
}

UINT GGDirectXDriver::GetResX() const
{
	return m_resX;
}

UINT GGDirectXDriver::GetResY() const
{
	return m_resY;
}

ID3D11Device* GGDirectXDriver::GetDevice() const
{
	return m_device.p;
}

ID3D11DeviceContext* GGDirectXDriver::GetDeviceContext() const
{
	return m_deviceContext.p;
}

IDXGISwapChain* GGDirectXDriver::GetSwapChain() const
{
	return m_swapChain.p;
}

ID2D1Factory* GGDirectXDriver::GetFactory2D() const
{
	return m_factory2d.p;
}