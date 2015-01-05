#pragma once

#include <DirectXMath.h>

class GGCamera
{
public:
	void Update( DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _rot );
	
private:
	DirectX::XMFLOAT4X4 m_viewMatrix;
};