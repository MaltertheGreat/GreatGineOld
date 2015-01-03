#pragma once

#include <string>

#include "GGDirectXDevice.h"

class GGShader
{
public:
	GGShader( const GGDirectXDevice& _device );

private:
	void LoadShader( std::string _fileName );

private:
	CComPtr<ID3D11VertexShader> m_vertexShader;
	CComPtr<ID3D11PixelShader> m_pixelShader;
};