#include "GGDirectXDevice.h"
#include "GGError.h"

GGDirectXDevice::GGDirectXDevice( const GGWindow& _window, UINT _resX, UINT _resY )
{
	if( _resX && _resY )
	{
		m_resX = _resX;
		m_resY = _resY;
	}
	else
	{
		m_resX = _window.GetWidth();
		m_resY = _window.GetHeight();
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
	sd.BufferDesc.Width = m_resX;
	sd.BufferDesc.Height = m_resY;
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
}

ID3D11Device * GGDirectXDevice::GetDevice() const
{
	return m_device.p;
}

ID3D11DeviceContext * GGDirectXDevice::GetDeviceContext() const
{
	return m_deviceContext;
}

IDXGISwapChain * GGDirectXDevice::GetSwapChain() const
{
	return m_swapChain;
}

UINT GGDirectXDevice::GetResolutionX() const
{
	return m_resX;
}

UINT GGDirectXDevice::GetResolutionY() const
{
	return m_resY;
}