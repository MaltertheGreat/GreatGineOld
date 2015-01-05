#include "GGRenderer.h"
#include "GGDirectXDriver.h"
#include "GGShader.h"
#include "GGMesh.h"
#include "GGError.h"


GGRenderer::GGRenderer( GGDirectXDriver& _driver )
	:
	m_device( _driver.GetDevice() ),
	m_deviceContext( _driver.GetDeviceContext() ),
	m_swapChain( _driver.GetSwapChain() )
{
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

	// Setup the viewport
	D3D11_VIEWPORT vp;
	vp.Width = (FLOAT) _driver.GetResX();
	vp.Height = (FLOAT) _driver.GetResY();
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	m_deviceContext->RSSetViewports( 1, &vp );

	m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
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

void GGRenderer::SetShader( const GGShader* _shader )
{
	m_deviceContext->VSSetShader( _shader->GetVertexShader(), nullptr, 0 );
	m_deviceContext->PSSetShader( _shader->GetPixelShader(), nullptr, 0 );
	m_deviceContext->IASetInputLayout( _shader->GetInputLayout() );

	return;
}

void GGRenderer::SetMesh( const GGMesh* _mesh )
{
	ID3D11Buffer* vertexBuffer = _mesh->GetVertexBuffer();
	UINT stride = sizeof( GGMesh::GGBasicVertex );
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers( 0, 1, &vertexBuffer, &stride, &offset );
	m_deviceContext->IASetIndexBuffer( _mesh->GetIndexBuffer(), DXGI_FORMAT_R16_UINT, offset );

	return;
}

void GGRenderer::RenderMesh( const GGMesh* _mesh )
{
	m_deviceContext->DrawIndexed( _mesh->GetIndexCount(), 0, 0 );

	return;
}