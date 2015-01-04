#include "GGDevice.h"
#include "GGDirectXDriver.h"
#include "GGBasicShader.h"
#include "GGError.h"

GGDevice::GGDevice( GGDirectXDriver& _driver )
	:
	m_device( _driver.GetDevice() ),
	m_deviceContext( _driver.GetDeviceContext() ),
	m_swapChain( _driver.GetSwapChain() )
{
	
}

GGBasicShader* GGDevice::CreateBasicShader()
{
	return new GGBasicShader( m_device );
}