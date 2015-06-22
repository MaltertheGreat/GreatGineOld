#include "PCH.h"
#include "GGDevice.h"
#include "GGDirectXDriver.h"
#include "GGMeshData.h"
#include "GGLinesData.h"
#include "GGError.h"
#include "Shaders\BasicVS.h"
#include "Shaders\BasicPS.h"
#include "Shaders\LinesVS.h"
#include "Shaders\LinesPS.h"

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
	float aspect = _viewWidth / static_cast<float>(_viewHeight);
	XMMATRIX projection = XMMatrixPerspectiveFovLH( XMConvertToRadians( _fovAngle ), aspect, 0.01f, 1000.0f );
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
	// Vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = static_cast<UINT>(sizeof( GGMeshData::GGVertex ) * _grid.vertices.size());
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
	bd.ByteWidth = static_cast<UINT>(sizeof( GGMeshData::GGIndex ) * _grid.indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	InitData.pSysMem = _grid.indices.data();

	ComPtr<ID3D11Buffer> indexBuffer;
	hr = m_device->CreateBuffer( &bd, &InitData, indexBuffer.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return GGMesh( static_cast<UINT>(_grid.indices.size()), vertexBuffer, indexBuffer );
}

GGMesh GGDevice::CraeteLinesMesh( const GGLinesData& _lines ) const
{
	// Vertex buffer
	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = static_cast<UINT>(sizeof( GGLinesData::GGVertex ) * _lines.vertices.size());
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = _lines.vertices.data();

	ComPtr<ID3D11Buffer> vertexBuffer;
	HRESULT hr = m_device->CreateBuffer( &bd, &InitData, vertexBuffer.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	// Index buffer
	bd.ByteWidth = static_cast<UINT>(sizeof( GGLinesData::GGIndex ) * _lines.indices.size());
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	InitData.pSysMem = _lines.indices.data();

	ComPtr<ID3D11Buffer> indexBuffer;
	hr = m_device->CreateBuffer( &bd, &InitData, indexBuffer.GetAddressOf() );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	return GGMesh( static_cast<UINT>(_lines.indices.size()), vertexBuffer, indexBuffer );
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