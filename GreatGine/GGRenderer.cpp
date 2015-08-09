#include "PCH.h"
#include "GGRenderer.h"
#include "GGDirectXDriver.h"
#include "GGShader.h"
#include "GGMesh.h"
#include "GGMeshData.h"
#include "GGLinesData.h"
#include "GGError.h"
using Microsoft::WRL::ComPtr;
using namespace DirectX;

GGRenderer::GGRenderer( GGDirectXDriver _driver, int _syncInterval )
	:
	m_device( _driver.device ),
	m_deviceContext( _driver.deviceContext ),
	m_swapChain( _driver.swapChain ),
	m_factory2d( _driver.factory2d ),
	m_syncInterval( _syncInterval )
{
	HRESULT hr;

	// Input-Assembler Stage
	{
		m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
	}

	// Vertex-Shader Stage
	{
		ComPtr<ID3D11Buffer> constantBuffer;

		D3D11_BUFFER_DESC bd;
		ZeroMemory( &bd, sizeof( bd ) );
		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.CPUAccessFlags = 0;

		// Per-camera constant buffer
		bd.ByteWidth = sizeof( GGConstantBufferPerCamera );

		hr = m_device->CreateBuffer( &bd, nullptr, &constantBuffer );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
		m_deviceContext->VSSetConstantBuffers( GGConstantBufferPerCamera::SLOT_NUMBER, 1, constantBuffer.GetAddressOf() );

		// Per-frame constant buffer
		bd.ByteWidth = sizeof( GGConstantBufferPerFrame );

		hr = m_device->CreateBuffer( &bd, nullptr, &constantBuffer );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
		m_deviceContext->VSSetConstantBuffers( GGConstantBufferPerFrame::SLOT_NUMBER, 1, constantBuffer.GetAddressOf() );

		// Per-frame constant buffer
		bd.ByteWidth = sizeof( GGConstantBufferPerMesh );

		hr = m_device->CreateBuffer( &bd, nullptr, &constantBuffer );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
		m_deviceContext->VSSetConstantBuffers( GGConstantBufferPerMesh::SLOT_NUMBER, 1, constantBuffer.GetAddressOf() );
	}

	// Rasterizer Stage
	{
		D3D11_RASTERIZER_DESC rasterDesc;
		rasterDesc.AntialiasedLineEnable = false;
		rasterDesc.CullMode = D3D11_CULL_BACK;
		rasterDesc.DepthBias = 0;
		rasterDesc.DepthBiasClamp = 0.0f;
		rasterDesc.DepthClipEnable = true;
		rasterDesc.FillMode = D3D11_FILL_SOLID;
		rasterDesc.FrontCounterClockwise = false;
		rasterDesc.MultisampleEnable = false;
		rasterDesc.ScissorEnable = false;
		rasterDesc.SlopeScaledDepthBias = 0.0f;

		hr = m_device->CreateRasterizerState( &rasterDesc, m_rasterizerState[ FILL_TYPE_SOLID ].GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		rasterDesc.CullMode = D3D11_CULL_NONE;
		rasterDesc.FillMode = D3D11_FILL_WIREFRAME;

		hr = m_device->CreateRasterizerState( &rasterDesc, m_rasterizerState[ FILL_TYPE_WIREFRAME ].GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		// Default rasterizer
		m_deviceContext->RSSetState( m_rasterizerState[ FILL_TYPE_SOLID ].Get() );

		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)_driver.resX;
		vp.Height = (FLOAT)_driver.resY;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		m_deviceContext->RSSetViewports( 1, &vp );
	}
	
	// Output-Merger Stage
	{
		ComPtr<ID3D11Texture2D> backBuffer;
		hr = m_swapChain->GetBuffer( 0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(backBuffer.GetAddressOf()) );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		hr = m_device->CreateRenderTargetView( backBuffer.Get(), nullptr, &m_renderTargetView );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		ComPtr<ID3D11Texture2D> depthStencil;
		D3D11_TEXTURE2D_DESC descDepth;
		ZeroMemory( &descDepth, sizeof( descDepth ) );
		descDepth.Width = _driver.resX;
		descDepth.Height = _driver.resY;
		descDepth.MipLevels = 1;
		descDepth.ArraySize = 1;
		descDepth.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
		descDepth.SampleDesc.Count = 1;
		descDepth.SampleDesc.Quality = 0;
		descDepth.Usage = D3D11_USAGE_DEFAULT;
		descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		descDepth.CPUAccessFlags = 0;
		descDepth.MiscFlags = 0;
		hr = m_device->CreateTexture2D( &descDepth, nullptr, &depthStencil );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		D3D11_DEPTH_STENCIL_VIEW_DESC descDSV;
		ZeroMemory( &descDSV, sizeof( descDSV ) );
		descDSV.Format = descDepth.Format;
		descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		descDSV.Texture2D.MipSlice = 0;
		hr = m_device->CreateDepthStencilView( depthStencil.Get(), &descDSV, &m_depthStencilView );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		m_deviceContext->OMSetRenderTargets( 1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get() );
	}
	
	/*
	 * Direct2D
	 */
	{
		ComPtr<IDXGISurface> backBuffer;
		hr = m_swapChain->GetBuffer( 0, IID_PPV_ARGS( &backBuffer ) );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		float dpiX;
		float dpiY;
		m_factory2d->GetDesktopDpi( &dpiX, &dpiY );

		// 2D render target
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties( D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat( DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_IGNORE ), dpiX, dpiY );
		hr = m_factory2d->CreateDxgiSurfaceRenderTarget( backBuffer.Get(), &props, m_renderTarget2D.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		m_renderTarget2D->SetTransform( D2D1::Matrix3x2F::Identity() );

		hr = m_renderTarget2D->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::LightSlateGray ), m_solidBrush.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(m_writeFactory.GetAddressOf()) );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}

		hr = m_writeFactory->CreateTextFormat( L"Candara", NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 18.0f, L"en-us", m_textFormat.GetAddressOf() );
		if( FAILED( hr ) )
		{
			GG_THROW;
		}
	}
}

void GGRenderer::ClearScene()
{
	float color[ 4 ] = { 0.2f, 0.2f, 0.8f, 1.0f };
	m_deviceContext->ClearRenderTargetView( m_renderTargetView.Get(), color );
	m_deviceContext->ClearDepthStencilView( m_depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0 );

	return;
}

void GGRenderer::PresentScene()
{
	m_renderTarget2D->EndDraw();
	//m_renderTarget2D->BeginDraw();
	//D2D1_RECT_F rectangle1 = D2D1::RectF( (m_resX / 2.0f) - 1.0f, (m_resY / 2.0f) - 1.0f, (m_resX / 2.0f) + 1.0f, (m_resY / 2.0f) + 1.0f );
	//m_renderTarget2D->FillRectangle( rectangle1, m_solidBrush.Get() );
	//m_renderTarget2D->EndDraw();

	m_swapChain->Present( m_syncInterval, 0 );

	return;
}

void GGRenderer::SetSyncInterval( int _syncInterval )
{
	m_syncInterval = _syncInterval;

	return;
}

void GGRenderer::SetRenderType( RENDER_TYPE _renderType )
{
	switch( _renderType )
	{
		case GGRenderer::RENDER_TYPE_MESH:
			m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );
			m_vertexSize = sizeof( GGMeshData::GGVertex );
			break;
		case GGRenderer::RENDER_TYPE_LINES:
			m_deviceContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );
			m_vertexSize = sizeof( GGLinesData::GGVertex );
			break;
	}

	return;
}

void GGRenderer::SetFillType( FILL_TYPE _fillType )
{
	m_deviceContext->RSSetState( m_rasterizerState[ _fillType ].Get() );

	return;
}

void GGRenderer::SetCamera( float _fovAngle, UINT _viewWidth, UINT _viewHeight )
{
	XMFLOAT4X4 projectionMatrix;
	float aspect = _viewWidth / static_cast<float>(_viewHeight);
	XMMATRIX projection = XMMatrixPerspectiveFovLH( XMConvertToRadians( _fovAngle ), aspect, 0.01f, 1000.0f );
	XMStoreFloat4x4( &projectionMatrix, XMMatrixTranspose( projection ) );

	GGConstantBufferPerCamera buffer = { projectionMatrix };
	UpdateConstantBuffer( GGConstantBufferPerCamera::SLOT_NUMBER, &buffer );

	return;
}

void GGRenderer::UpdateCamera( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation )
{
	XMVECTOR position = XMLoadFloat3( &_position );
	XMVECTOR rotation = XMLoadFloat3( &_rotation );
	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );	// Convert rotation vector to quaternion
	XMVECTOR lookDir = XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), rotation );
	XMVECTOR upDir = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	XMFLOAT4X4 viewMatrix;
	XMMATRIX view = XMMatrixLookToLH( position, lookDir, upDir );
	XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( view ) );

	GGConstantBufferPerFrame buffer = { viewMatrix, { _position.x , _position.y, _position.z, 1.0f } };
	UpdateConstantBuffer( GGConstantBufferPerFrame::SLOT_NUMBER, &buffer );

	return;
}

void GGRenderer::SetShader( const GGShader& _shader )
{
	m_deviceContext->IASetInputLayout( _shader.GetInputLayout().Get() );
	m_deviceContext->VSSetShader( _shader.GetVertexShader().Get(), nullptr, 0 );
	m_deviceContext->PSSetShader( _shader.GetPixelShader().Get(), nullptr, 0 );

	return;
}

void GGRenderer::SetMesh( const GGMesh& _mesh )
{
	UINT stride = m_vertexSize;
	UINT offset = 0;
	m_deviceContext->IASetVertexBuffers( 0, 1, _mesh.GetVertexBuffer().GetAddressOf(), &stride, &offset );
	m_deviceContext->IASetIndexBuffer( _mesh.GetIndexBuffer().Get(), DXGI_FORMAT_R32_UINT, offset );

	return;
}

void GGRenderer::RenderIn2D()
{
	m_renderTarget2D->BeginDraw();

	return;
}

void GGRenderer::RenderMesh( const GGMesh _mesh, XMFLOAT4X4 _transform )
{
	XMStoreFloat4x4( &_transform, XMMatrixTranspose( XMLoadFloat4x4( &_transform ) ) );

	UpdateConstantBuffer( GGConstantBufferPerMesh::SLOT_NUMBER, _transform.m );

	m_deviceContext->DrawIndexed( _mesh.GetIndexCount(), 0, 0 );

	return;
}

void GGRenderer::RenderText( const std::wstring& _text, const XMFLOAT2& _pos )
{
	D2D1_RECT_F rectangle1 = D2D1::RectF( _pos.x, _pos.y, _pos.x + 512.0f, _pos.y + 18.0f );
	m_renderTarget2D->DrawText( _text.c_str(), static_cast<UINT32>(_text.length()), m_textFormat.Get(), rectangle1, m_solidBrush.Get() );

	return;
}

void GGRenderer::UpdateConstantBuffer( UINT _slot, const void* _data )
{
	ComPtr<ID3D11Buffer> constantBuffer;
	m_deviceContext->VSGetConstantBuffers( _slot, 1, constantBuffer.GetAddressOf() );

	m_deviceContext->UpdateSubresource( constantBuffer.Get(), 0, nullptr, _data, 0, 0 );

	return;
}