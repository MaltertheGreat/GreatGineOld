#include "PCH.h"
#include "GGShader.h"
using Microsoft::WRL::ComPtr;

GGShader::GGShader( ComPtr<ID3D11VertexShader> _vertexShader, ComPtr<ID3D11PixelShader> _pixelShader, ComPtr<ID3D11InputLayout> _inputLayout )
	:
	m_vertexShader( _vertexShader ),
	m_pixelShader( _pixelShader ),
	m_inputLayout( _inputLayout )
{}

/*GGShader::GGShader( GGShader&& _from )
{
m_vertexShader.Attach( _from.m_vertexShader.Detach() );
m_pixelShader.Attach( _from.m_pixelShader.Detach() );
m_inputLayout.Attach( _from.m_inputLayout.Detach() );
}*/

ComPtr<ID3D11VertexShader> GGShader::GetVertexShader() const
{
	return m_vertexShader;
}

ComPtr<ID3D11PixelShader> GGShader::GetPixelShader() const
{
	return m_pixelShader;
}

ComPtr<ID3D11InputLayout> GGShader::GetInputLayout() const
{
	return m_inputLayout;
}