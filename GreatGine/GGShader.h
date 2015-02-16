#pragma once

#include "PCH.h"

class GGShader
{
public:
	GGShader( Microsoft::WRL::ComPtr<ID3D11VertexShader> _vertexShader, Microsoft::WRL::ComPtr<ID3D11PixelShader> _pixelShader, Microsoft::WRL::ComPtr<ID3D11InputLayout> _inputLayout );

	Microsoft::WRL::ComPtr<ID3D11VertexShader> GetVertexShader() const;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> GetPixelShader() const;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> GetInputLayout() const;

private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout;
};