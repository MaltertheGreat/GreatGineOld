#pragma once

#include "PCH.h"

struct GGDirectXDriver;
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
	GGRenderer( GGDirectXDriver _driver, int _syncInterval = 0 );

public:
	void ClearScene();
	void PresentScene();

	void SetSyncInterval( int _syncInterval );
	void SetRenderType( RENDER_TYPE _renderType );
	void SetFillType( FILL_TYPE _fillType );
	void SetCamera( float _fovAngle, UINT _viewWidth, UINT _viewHeight );
	void UpdateCamera( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation );

	void SetShader( const GGShader& _shader );
	void SetMesh( const GGMesh& _mesh );

	void RenderMesh( const GGMesh _mesh, DirectX::XMFLOAT4X4 _transform );

	void Begin2DRendering();
	void End2DRendering();
	void RenderText( const std::wstring& _text, const DirectX::XMFLOAT2& _pos, const D2D1::ColorF& _color = D2D1::ColorF::LightSlateGray );
	void RenderRectangle( const DirectX::XMFLOAT2& _pos, const DirectX::XMFLOAT2& _size, const D2D1::ColorF& _color = D2D1::ColorF::LightSlateGray );
	void RenderEllipse( const DirectX::XMFLOAT2& _pos, float _radiusX, float _radiusY, const D2D1::ColorF& _color = D2D1::ColorF::LightSlateGray );

private:
	struct GGConstantBufferPerCamera
	{
		constexpr static UINT SLOT_NUMBER = 0;

		DirectX::XMFLOAT4X4 projectionMatrix;
	};
	struct GGConstantBufferPerFrame
	{
		constexpr static UINT SLOT_NUMBER = 1;

		DirectX::XMFLOAT4X4 viewMatrix;
		DirectX::XMFLOAT4 cameraPos;
	};
	struct GGConstantBufferPerMesh
	{
		constexpr static UINT SLOT_NUMBER = 2;

		DirectX::XMFLOAT4X4 worldMatrix;
	};

	void UpdateConstantBuffer( UINT _slot, const void* _data );

private:
	int m_syncInterval;
	UINT m_vertexSize;

	Microsoft::WRL::ComPtr<ID3D11Device> m_device;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_deviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain> m_swapChain;
	Microsoft::WRL::ComPtr<ID2D1Factory> m_factory2d;

	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_rasterizerState[ FILL_TYPE_COUNT ];
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_depthStencilView;

	Microsoft::WRL::ComPtr<ID2D1RenderTarget> m_renderTarget2D;
	Microsoft::WRL::ComPtr<IDWriteFactory> m_writeFactory;
	Microsoft::WRL::ComPtr<IDWriteTextFormat> m_textFormat;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_solidBrush;
};