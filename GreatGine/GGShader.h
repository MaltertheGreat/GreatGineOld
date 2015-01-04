#pragma once

#include <d3d11.h>

class GGShader
{
public:
	virtual void BindToPipeline( ID3D11DeviceContext* _deviceContext ) = 0;
};