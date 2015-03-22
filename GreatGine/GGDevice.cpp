#include "PCH.h"
#include "GGDevice.h"
#include "GGDirectXDriver.h"
#include "GGMeshData.h"
#include "GGError.h"
#include "Shaders/BasicVS.h"
#include "Shaders/BasicPS.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

GGDevice::GGDevice( GGDirectXDriver& _driver )
	:
	m_device( _driver.GetDevice() ),
	m_deviceContext( _driver.GetDeviceContext() ),
	m_swapChain( _driver.GetSwapChain() )
{}

GGCamera GGDevice::CreateCamera( float _fovAngle, UINT _viewWidth, UINT _viewHeight ) const
{
	// View matrix constant buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( XMFLOAT4X4 );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	ComPtr<ID3D11Buffer> viewBuffer;
	HRESULT hr = m_device->CreateBuffer( &bd, nullptr, viewBuffer.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	XMFLOAT4X4 projectionMatrix;
	XMMATRIX projection = XMMatrixPerspectiveFovLH( _fovAngle, _viewWidth / static_cast<float>(_viewHeight), 0.01f, 1000.0f );
	XMStoreFloat4x4( &projectionMatrix, XMMatrixTranspose( projection ) );

	// Projection matrix constant buffer
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( XMFLOAT4X4 );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = projectionMatrix.m;

	ComPtr<ID3D11Buffer> projectionBuffer;
	hr = m_device->CreateBuffer( &bd, &InitData, projectionBuffer.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return GGCamera( viewBuffer, projectionBuffer );
}

GGShader GGDevice::CreateShader() const
{
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> inputLayout;

	HRESULT hr = m_device->CreateVertexShader( g_vertexShader, sizeof( g_vertexShader ), nullptr, vertexShader.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	hr = m_device->CreatePixelShader( g_pixelShader, sizeof( g_pixelShader ), nullptr, pixelShader.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT numElements = ARRAYSIZE( layout );

	hr = m_device->CreateInputLayout( layout, numElements, g_vertexShader, sizeof( g_vertexShader ), inputLayout.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return GGShader( vertexShader, pixelShader, inputLayout );
}

std::unique_ptr<GGMesh> GGDevice::CreateMesh( const GGMeshData& _grid ) const
{
	// Vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( GGMeshData::GGVertex ) * _grid.vertices.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = _grid.vertices.data();

	ComPtr<ID3D11Buffer> vertexBuffer;
	HRESULT hr = m_device->CreateBuffer( &bd, &InitData, vertexBuffer.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	// Index buffer
	bd.ByteWidth = sizeof( GGMeshData::GGIndex ) * _grid.indices.size();
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	InitData.pSysMem = _grid.indices.data();

	ComPtr<ID3D11Buffer> indexBuffer;
	hr = m_device->CreateBuffer( &bd, &InitData, indexBuffer.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return std::make_unique<GGMesh>( _grid.indices.size(), vertexBuffer, indexBuffer );
}

void GGDevice::UpdateCamera( GGCamera& _camera, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation ) const
{
	XMVECTOR position = XMLoadFloat3( &_position );
	XMVECTOR rotation = XMLoadFloat3( &_rotation );
	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );	// Convert rotation vector to quaternion
	XMVECTOR lookDir = XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), rotation );
	XMVECTOR upDir = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	XMFLOAT4X4 viewMatrix;
	XMMATRIX view = XMMatrixLookToLH( position, lookDir, upDir );
	XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( view ) );

	m_deviceContext->UpdateSubresource( _camera.GetViewBuffer().Get(), 0, nullptr, viewMatrix.m, 0, 0 );

	return;
}