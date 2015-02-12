#include "GGGraphics.h"
#include "GGWindow.h"
#include <DirectXMath.h>
using namespace DirectX;

GGGraphics::GGGraphics( const GGWindow& _window )
	:
	m_driver( _window, 800, 600 ),
	m_device( m_driver ),
	m_renderer( m_driver ),
	m_camera( m_device.CreateCamera( XMConvertToRadians( 80.0f ) ) ),
	m_basicShader( m_device.CreateShader() ),
	m_mesh( m_device.CreateCubeMesh() )
{}

void GGGraphics::Update( float _frameTime )
{
	float pitch = XMConvertToRadians( m_cameraRot.x );
	float yaw = XMConvertToRadians( m_cameraRot.y );
	float roll = XMConvertToRadians( m_cameraRot.z );
	XMVECTOR rotationQuaterion = XMQuaternionRotationRollPitchYaw( pitch, yaw, roll );

	XMVECTOR velocity = XMLoadFloat3( &m_cameraVelocity );
	velocity = XMVector3Rotate( velocity, rotationQuaterion );
	XMVECTOR position = XMLoadFloat3( &m_cameraPos );
	position = XMVectorAdd( position, XMVectorScale( velocity, _frameTime ) );

	XMStoreFloat3( &m_cameraPos, position );

	XMVECTOR lookDir = XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f ), rotationQuaterion );
	XMVECTOR upDir = XMVectorSet( 0.0f, 1.0f, 0.0f, 1.0f );

	XMFLOAT4X4 viewMatrix;
	XMMATRIX view = XMMatrixLookToLH( position, lookDir, upDir );
	XMStoreFloat4x4( &viewMatrix, XMMatrixTranspose( view ) );

	m_device.UpdateCamera( m_camera, viewMatrix );

	return;
}

void GGGraphics::Render()
{
	m_renderer.ClearScene();

	m_renderer.SetCamera( m_camera );

	m_renderer.SetShader( m_basicShader );

	m_renderer.SetMesh( m_mesh );
	m_renderer.RenderMesh( m_mesh );

	m_renderer.PresentScene();

	return;
}

void GGGraphics::HandleActionInput( GG_ACTION_INPUT _input, bool _down )
{
	if( _down )
	{
		if( _input == GG_ACTION_INPUT_MOVE_FORWARD )
		{
			m_cameraVelocity.z = 4.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_BACKWARD )
		{
			m_cameraVelocity.z = -4.0f;
		}

		if( _input == GG_ACTION_INPUT_MOVE_RIGHTWARD )
		{
			m_cameraVelocity.x = 4.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_LEFTWARD )
		{
			m_cameraVelocity.x = -4.0f;
		}
	}
	else
	{
		if( _input == GG_ACTION_INPUT_MOVE_FORWARD )
		{
			m_cameraVelocity.z = 0.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_BACKWARD )
		{
			m_cameraVelocity.z = 0.0f;
		}

		if( _input == GG_ACTION_INPUT_MOVE_RIGHTWARD )
		{
			m_cameraVelocity.x = 0.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_LEFTWARD )
		{
			m_cameraVelocity.x = 0.0f;
		}
	}

	return;
}

void GGGraphics::HandleRangeInput( int _x, int _y )
{
	m_cameraRot.x += _y * 0.1f;
	if( m_cameraRot.x >= 89.9f )
	{
		m_cameraRot.x = 89.9f;
	}
	else if( m_cameraRot.x <= -89.9f )
	{
		m_cameraRot.x = -89.9f;
	}

	m_cameraRot.y += _x * 0.1f;

	return;
}