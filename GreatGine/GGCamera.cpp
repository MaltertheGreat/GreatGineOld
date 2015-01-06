#include "GGCamera.h"
using namespace DirectX;

void GGCamera::Update( DirectX::XMFLOAT3 _pos, DirectX::XMFLOAT3 _rot )
{
	XMVECTOR eyePos = XMLoadFloat3( &_pos );

	float pitch = XMConvertToRadians( _rot.x );
	float yaw = XMConvertToRadians( _rot.y);
	float roll = XMConvertToRadians( _rot.z );
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw( pitch, yaw, roll );

	XMVECTOR focusPos = XMVectorSet( 0.0f, 0.0f, 1.0f, 1.0f );
	XMVECTOR upDir = XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f );

	focusPos = XMVector3TransformCoord( focusPos, rotation );
	upDir = XMVector3TransformCoord( upDir, rotation );

	XMStoreFloat4x4( &m_viewMatrix, XMMatrixLookAtLH( eyePos, focusPos, upDir ) );

	return;
}

DirectX::XMFLOAT4X4 GGCamera::GetViewMatrix() const
{
	return m_viewMatrix;
}