#include "GGShader.h"

GGShader::GGShader( ID3D11VertexShader* _vertexShader, ID3D11PixelShader* _pixelShader, ID3D11InputLayout* _inputLayout )
	:
	m_vertexShader( _vertexShader ),
	m_pixelShader( _pixelShader ),
	m_inputLayout( _inputLayout )
{
}

GGShader::~GGShader()
{
	m_vertexShader.Release();
	m_pixelShader.Release();
	m_inputLayout.Release();
}

ID3D11VertexShader* GGShader::GetVertexShader() const
{
	return m_vertexShader.p;
}

ID3D11PixelShader* GGShader::GetPixelShader() const
{
	return m_pixelShader.p;
}

ID3D11InputLayout* GGShader::GetInputLayout() const
{
	return m_inputLayout.p;
}