#include "GGRenderer.h"
#include "GGDirectXDriver.h"
#include "GGCamera.h"
#include "GGShader.h"
#include "GGMesh.h"
#include "GGError.h"
using namespace DirectX;

GGRenderer::GGRenderer( GGDirectXDriver& _driver )
	:
	m_device( _driver.GetDevice() ),
	m_deviceContext( _driver.GetDeviceContext() ),
	m_swapChain( _driver.GetSwapChain() )
{
	// Back buffer creation
	CComPtr<ID3D11Texture2D> backBuffer;
	HRESULT hr = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer) );
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

	D3D11_RASTERIZER_DESC rasterDesc;
	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_NONE;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	hr = m_device->CreateRasterizerState( &rasterDesc, &m_rasterizerState );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	m_deviceContext->RSSetState( m_rasterizerState );

	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT) _driver.GetResX();
	vp.Height = (FLOAT) _driver.GetResY();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports( 1, &vp );

	// For now lets assume all meshes are triangulated ;)
	m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

	// World constant buffer
	XMFLOAT4X4 worldMatrix;
	XMStoreFloat4x4( &worldMatrix, XMMatrixTranspose( XMMatrixIdentity() ) );

	D3D11_BUFFER_DESC bd;
	ZeroMemory( &bd, sizeof( bd ) );
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof( XMFLOAT4X4 );
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.ByteWidth = sizeof( XMFLOAT4X4 );

	D3D11_SUBRESOURCE_DATA InitData;
	ZeroMemory( &InitData, sizeof( InitData ) );
	InitData.pSysMem = worldMatrix.m;

	hr = m_device->CreateBuffer( &bd, &InitData, &m_worldBuffer );
	if( FAILED( hr ) )
	{
		GG_THROW;
	}

	m_deviceContext->VSSetConstantBuffers( 0, 1, &m_worldBuffer.p );
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

void GGRenderer::SetCamera( const GGCamera& _camera )
{
	/*XMFLOAT4X4 viewMatrix = _camera->GetViewMatrix();

	XMMATRIX view = XMLoadFloat4x4( &viewMatrix );
	XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( view ) );

	m_deviceContext->UpdateSubresource( m_viewBuffer, 0, nullptr, viewMatrix.m, 0, 0 );*/

	UINT viewBufferSlot = 1;
	UINT projectionBufferSlot = 2;
	ID3D11Buffer* viewBuffer = _camera.GetViewBuffer();
	ID3D11Buffer* projectionBuffer = _camera.GetProjectionBuffer();

	m_deviceContext->VSSetConstantBuffers( viewBufferSlot, 1, &viewBuffer );
	m_deviceContext->VSSetConstantBuffers( projectionBufferSlot, 1, &projectionBuffer );

	return;
}

void GGRenderer::SetShader( const GGShader& _shader )
{
	m_deviceContext->VSSetShader( _shader.GetVertexShader(), nullptr, 0 );
	m_deviceContext->PSSetShader( _shader.GetPixelShader(), nullptr, 0 );
	m_deviceContext->IASetInputLayout( _shader.GetInputLayout() );

	return;
}

void GGRenderer::SetMesh( const GGMesh& _mesh )
{
	ID3D11Buffer* vertexBuffer = _mesh.GetVertexBuffer();
	UINT stride = sizeof( GGMesh::GGBasicVertex );
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
	m_deviceContext->IASetIndexBuffer( _mesh.GetIndexBuffer(), DXGI_FORMAT_R16_UINT, offset );

	return;
}

void GGRenderer::RenderMesh( const GGMesh& _mesh )
{
	m_deviceContext->DrawIndexed( _mesh.GetIndexCount(), 0, 0 );

	return;
}