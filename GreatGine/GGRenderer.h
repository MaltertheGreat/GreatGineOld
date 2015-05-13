#pragma once

#include "PCH.h"

class GGDirectXDriver;
class GGCamera;
class GGShader;
class GGMesh;
class GGLines;

class GGRenderer
{
public:
	enum RENDER_TYPE
	{
		RENDER_TYPE_MESH,
		RENDER_TYPE_LINES,

		RENDER_TYPE_COUNT
	};

	enum FILL_TYPE
	{
		FILL_TYPE_SOLID,
		FILL_TYPE_WIREFRAME,

		FILL_TYPE_COUNT
	};
public:
	GGRenderer( GGDirectXDriver& _driver, int _syncInterval = 0 );

public:
	void ClearScene();
	void PresentScene();

	void SetSyncInterval( int _syncInterval );
	void SetRenderType( RENDER_TYPE _renderType );
	void SetFillType( FILL_TYPE _fillType );

	void SetCamera( const GGCamera& _camera );
	void SetShader( const GGShader& _shader );
	void SetMesh( const GGMesh& _mesh );

	void RenderIn2D();
	void RenderMesh( const GGMesh _mesh, const DirectX::XMFLOAT4X4& _transform );
	void RenderText( const std::wstring& _text, const DirectX::XMFLOAT2& _pos );

private:
	int m_syncInterval;
	UINT m_vertexSize;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID2D1Factory> m_factory2d;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState[ FILL_TYPE_COUNT ];
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_depthStencil;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_worldBuffer;
	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_renderTarget2D;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_solidBrush;
	Microsoft::WRL::ComPtr<IDWriteFactory> m_writeFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
};