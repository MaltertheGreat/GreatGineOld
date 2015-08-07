#include "PCH.h"
#include "GGDevice.h"
#include "GGWindow.h"
#include "GGLinesData.h"
#include "GGError.h"
#include "Shaders\BasicVS.h"
#include "Shaders\BasicPS.h"
#include "Shaders\LinesVS.h"
#include "Shaders\LinesPS.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;
using namespace std;

GGDevice::GGDevice( const GGWindow& _window, UINT _resX, UINT _resY )
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

	HRESULT hr = D3D11CreateDeviceAndSwapChain( nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevels, numFeatureLevels, D3D11_SDK_VERSION, &sd, m_swapChain.GetAddressOf(), m_device.GetAddressOf(), nullptr, m_deviceContext.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, static_cast<ID2D1Factory**>(m_factory2d.GetAddressOf()) );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}
}

GGDirectXDriver GGDevice::GetDriver()
{
	return GGDirectXDriver{ m_resX, m_resY, m_device, m_deviceContext, m_swapChain, m_factory2d };
}

GGShader GGDevice::CreateShader() const
{
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	HRESULT hr = m_device->CreateVertexShader( g_basicVS, sizeof( g_basicVS ), nullptr, vertexShader.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = m_device->CreatePixelShader( g_basicPS, sizeof( g_basicPS ), nullptr, pixelShader.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	hr = m_device->CreateInputLayout( layout, numElements, g_basicVS, sizeof( g_basicVS ), inputLayout.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return GGShader( vertexShader, pixelShader, inputLayout );
}

GGShader GGDevice::CreateLinesShader() const
{
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	HRESULT hr = m_device->CreateVertexShader( g_linesVS, sizeof( g_linesVS ), nullptr, vertexShader.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = m_device->CreatePixelShader( g_linesPS, sizeof( g_linesPS ), nullptr, pixelShader.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	UINT numElements = ARRAYSIZE( layout );

	hr = m_device->CreateInputLayout( layout, numElements, g_linesVS, sizeof( g_linesVS ), inputLayout.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return GGShader( vertexShader, pixelShader, inputLayout );
}

GGMesh GGDevice::CreateMesh( const GGMeshData& _grid ) const
{
	ComPtr<ID3D11Buffer> vertexBuffer = CreateVertexBuffer( sizeof( GGMeshData::GGVertex ) * _grid.vertices.size(), _grid.vertices.data() );
	ComPtr<ID3D11Buffer> indexBuffer = CreateIndexBuffer( sizeof( GGMeshData::GGIndex ) * _grid.indices.size(), _grid.indices.data() );


	return GGMesh( static_cast<UINT>(_grid.indices.size()), vertexBuffer, indexBuffer );
}

GGMesh GGDevice::CraeteLinesMesh( const GGLinesData& _lines ) const
{
	ComPtr<ID3D11Buffer> vertexBuffer = CreateVertexBuffer( sizeof( GGLinesData::GGVertex ) * _lines.vertices.size(), _lines.vertices.data() );
	ComPtr<ID3D11Buffer> indexBuffer = CreateIndexBuffer( sizeof( GGLinesData::GGIndex ) * _lines.indices.size(), _lines.indices.data() );


	return GGMesh( static_cast<UINT>(_lines.indices.size()), vertexBuffer, indexBuffer );
}


ComPtr<ID3D11Buffer> GGDevice::CreateVertexBuffer( UINT _size, const void* _data ) const
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = _size;
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ComPtr<ID3D11Buffer> vertexBuffer;

	if( _data )
	{
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory( &initData, sizeof( initData ) );
		initData.pSysMem = _data;

		HRESULT hr = m_device->CreateBuffer( &bd, &initData, vertexBuffer.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
	}
	else
	{
		HRESULT hr = m_device->CreateBuffer( &bd, nullptr, vertexBuffer.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
	}

	return vertexBuffer;
}

ComPtr<ID3D11Buffer> GGDevice::CreateIndexBuffer( UINT _size, const void* _data ) const
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = _size;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;

	ComPtr<ID3D11Buffer> indexBuffer;

	if( _data )
	{
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory( &initData, sizeof( initData ) );
		initData.pSysMem = _data;

		HRESULT hr = m_device->CreateBuffer( &bd, &initData, indexBuffer.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
	}
	else
	{
		HRESULT hr = m_device->CreateBuffer( &bd, nullptr, indexBuffer.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
	}

	return indexBuffer;
}

ComPtr<ID3D11Buffer> GGDevice::CreateConstantBuffer( UINT _size, const void* _data ) const
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = _size;
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ComPtr<ID3D11Buffer> constantBuffer;

	if( _data )
	{
		D3D11_SUBRESOURCE_DATA initData;
		ZeroMemory( &initData, sizeof( initData ) );
		initData.pSysMem = _data;

		HRESULT hr = m_device->CreateBuffer( &bd, &initData, constantBuffer.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
	}
	else
	{
		HRESULT hr = m_device->CreateBuffer( &bd, nullptr, constantBuffer.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
	}

	return constantBuffer;
}
