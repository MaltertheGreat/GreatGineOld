#pragma once

#include <d3d11.h>
#include <d2d1.h>
#include <Dwrite.h>
#include <atlbase.h>
#include <DirectXMath.h>
#include <string>

class GGDirectXDriver;
class GGCamera;
class GGShader;
class GGMesh;

class GGRenderer
{
public:
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
	void SetFillType( FILL_TYPE _fillType );
	void SetCamera( const GGCamera& _camera );
	void SetShader( const GGShader& _shader );
	void SetMesh( const GGMesh& _mesh );

	void RenderIn2D();
	void RenderMesh( const GGMesh& _mesh );
	void RenderText( const std::wstring& _text );

private:
	int m_syncInterval;

	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;
	ID2D1Factory* m_factory2d;

	CComPtr<ID3D11RenderTargetView> m_renderTargetView;
	CComPtr<ID3D11RasterizerState> m_rasterizerState[ FILL_TYPE_COUNT ];
	CComPtr<ID3D11Buffer> m_worldBuffer;
	CComPtr<ID2D1RenderTarget> m_renderTarget2D;
	CComPtr<ID2D1SolidColorBrush> m_solidBrush;
	CComPtr<IDWriteFactory> m_writeFactory;
	CComPtr<IDWriteTextFormat> m_textFormat;
};