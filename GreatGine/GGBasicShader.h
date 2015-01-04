#pragma once

#include <d3d11.h>
#include <atlbase.h>

#include "GGShader.h"


class GGBasicShader : public GGShader
{
public:
	GGBasicShader( ID3D11Device* _device );

public:
	virtual void BindToPipeline( ID3D11DeviceContext* _deviceContext ) override;

private:
	CComPtr<ID3D11VertexShader> m_vertexShader;
	CComPtr<ID3D11PixelShader> m_pixelShader;
	CComPtr<ID3D11InputLayout> m_inputLayout;
};