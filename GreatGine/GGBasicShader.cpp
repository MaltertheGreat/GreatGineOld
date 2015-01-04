#include "GGBasicShader.h"
#include "GGError.h"
#include "Shaders\BasicVS.h"
#include "Shaders\BasicPS.h"


GGBasicShader::GGBasicShader( ID3D11Device* _device )
{

	HRESULT hr = _device->CreateVertexShader( g_vertexShader, sizeof( g_vertexShader ), nullptr, &m_vertexShader );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = _device->CreatePixelShader( g_pixelShader, sizeof( g_pixelShader ), nullptr, &m_pixelShader );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( layout );

	hr = _device->CreateInputLayout( layout, numElements, g_vertexShader, sizeof( g_vertexShader ), &m_inputLayout );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}
}

void GGBasicShader::BindToPipeline( ID3D11DeviceContext* _deviceContext )
{
	_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	_deviceContext->VSSetShader( m_vertexShader, nullptr, 0 );
	_deviceContext->PSSetShader( m_pixelShader, nullptr, 0 );
	_deviceContext->IASetInputLayout( m_inputLayout );

	return;
}