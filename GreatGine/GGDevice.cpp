#include "GGDevice.h"
#include "GGDirectXDriver.h"
#include "GGShader.h"
#include "GGMesh.h"
#include "GGError.h"
#include "Shaders/BasicVS.h"
#include "Shaders/BasicPS.h"

#include <atlbase.h>
#include <DirectXMath.h>
using namespace DirectX;

GGDevice::GGDevice( GGDirectXDriver& _driver )
	:
	m_device( _driver.GetDevice() ),
	m_deviceContext( _driver.GetDeviceContext() ),
	m_swapChain( _driver.GetSwapChain() )
{ }

GGDevice::~GGDevice()
{ }


GGShader* GGDevice::CreateShader()
{
	CComPtr<ID3D11VertexShader> vertexShader;
	CComPtr<ID3D11PixelShader> pixelShader;
	CComPtr<ID3D11InputLayout> inputLayout;

	HRESULT hr = m_device->CreateVertexShader( g_vertexShader, sizeof( g_vertexShader ), nullptr, &vertexShader );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = m_device->CreatePixelShader( g_pixelShader, sizeof( g_pixelShader ), nullptr, &pixelShader );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( layout );

	hr = m_device->CreateInputLayout( layout, numElements, g_vertexShader, sizeof( g_vertexShader ), &inputLayout );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return new GGShader( vertexShader, pixelShader, inputLayout	);
}

GGMesh* GGDevice::CreateTriangleMesh()
{
	// Vertex buffer
	GGMesh::GGBasicVertex vertices[] =
	{
		XMFLOAT3( 0.0f, 0.5f, 0.5f ),
		XMFLOAT3( 0.5f, -0.5f, 0.5f ),
		XMFLOAT3( -0.5f, -0.5f, 0.5f ),
	};
	UINT vertexCount = ARRAYSIZE( vertices );

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( GGMesh::GGBasicVertex ) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = vertices;

	CComPtr<ID3D11Buffer> vertexBuffer;
	HRESULT hr = m_device->CreateBuffer( &bd, &InitData, &vertexBuffer );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	// Index buffer
	WORD indices[] = { 0, 1, 2 };
	UINT indexCount = ARRAYSIZE( indices );

	bd.ByteWidth = sizeof( WORD ) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	InitData.pSysMem = indices;
	CComPtr<ID3D11Buffer> indexBuffer;
	hr = m_device->CreateBuffer( &bd, &InitData, &indexBuffer );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return new GGMesh( indexCount, vertexBuffer, indexBuffer );
}

GGMesh* GGDevice::CreateCubeMesh()
{
	// Vertex buffer
	GGMesh::GGBasicVertex vertices[] =
	{
		XMFLOAT3( -1.0f, 1.0f, -1.0f ),
		XMFLOAT3( 1.0f, 1.0f, -1.0f ),
		XMFLOAT3( 1.0f, 1.0f, 1.0f ),
		XMFLOAT3( -1.0f, 1.0f, 1.0f ),
		XMFLOAT3( -1.0f, -1.0f, -1.0f ),
		XMFLOAT3( 1.0f, -1.0f, -1.0f ),
		XMFLOAT3( 1.0f, -1.0f, 1.0f ),
		XMFLOAT3( -1.0f, -1.0f, 1.0f )
	};
	UINT vertexCount = ARRAYSIZE( vertices );

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( GGMesh::GGBasicVertex ) * vertexCount;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = vertices;

	CComPtr<ID3D11Buffer> vertexBuffer;
	HRESULT hr = m_device->CreateBuffer( &bd, &InitData, &vertexBuffer );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	// Index buffer
	WORD indices[] =
	{
		3, 1, 0,
		2, 1, 3,

		0, 5, 4,
		1, 5, 0,

		3, 4, 7,
		0, 4, 3,

		1, 6, 5,
		2, 6, 1,

		2, 7, 6,
		3, 7, 2,

		6, 4, 5,
		7, 4, 6,
	};
	UINT indexCount = ARRAYSIZE( indices );

	bd.ByteWidth = sizeof( WORD ) * indexCount;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	InitData.pSysMem = indices;
	CComPtr<ID3D11Buffer> indexBuffer;
	hr = m_device->CreateBuffer( &bd, &InitData, &indexBuffer );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return new GGMesh( indexCount, vertexBuffer, indexBuffer );
}