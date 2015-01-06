#pragma once

#include <DirectXMath.h>

class GGCamera
{
public:
	void Update( DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _rot );

	DirectX::XMFLOAT4X4 GetViewMatrix() const;
	
private:
	DirectX::XMFLOAT4X4 m_viewMatrix;
};