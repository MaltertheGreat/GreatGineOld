#pragma once

#include <d3d11.h>
#include <atlbase.h>

class GGShader
{
public:
	GGShader( ID3D11VertexShader* _vertexShader, ID3D11PixelShader* _pixelShader, ID3D11InputLayout* _inputLayout );
	~GGShader();

	ID3D11VertexShader* GetVertexShader() const;
	ID3D11PixelShader* GetPixelShader() const;
	ID3D11InputLayout* GetInputLayout() const;
	
private:
	CComPtr<ID3D11VertexShader> m_vertexShader;
	CComPtr<ID3D11PixelShader> m_pixelShader;
	CComPtr<ID3D11InputLayout> m_inputLayout;
};