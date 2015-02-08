#pragma once

#include <d3d11.h>
#include <atlbase.h>
#include <DirectXMath.h>

class GGDirectXDriver;
class GGCamera;
class GGShader;
class GGMesh;

class GGRenderer
{
public:
	GGRenderer( GGDirectXDriver& _driver );

public:
	void ClearScene();
	void PresentScene();

	void SetCamera( const GGCamera& _camera );
	void SetShader( const GGShader& _shader );
	void SetMesh( const GGMesh& _mesh );

	void RenderMesh( const GGMesh& _mesh );

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
	IDXGISwapChain* m_swapChain;

	CComPtr<ID3D11RenderTargetView> m_renderTargetView;
	CComPtr<ID3D11RasterizerState> m_rasterizerState;
	CComPtr<ID3D11Buffer> m_worldBuffer;
};