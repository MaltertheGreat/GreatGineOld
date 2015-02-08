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

void GGGraphics::Update()
{
	m_cameraPos.x += m_cameraVelocity.x;
	m_cameraPos.z += m_cameraVelocity.z;

	XMVECTOR eyePos = XMLoadFloat3( &m_cameraPos );
	XMVECTOR lookDir = XMVectorSet( 0.0f, 0.0f, 1.0f, 0.0f );
	XMVECTOR upDir = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );

	XMFLOAT4X4 viewMatrix;
	XMMATRIX view = XMMatrixLookToLH( eyePos, lookDir, upDir );
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
	if( !_down )
	{
		return;
	}

	if( _input == GG_ACTION_INPUT_MOVE_FORWARD )
	{
		m_cameraVelocity.z += 0.00001f;
	}
	else if( _input == GG_ACTION_INPUT_MOVE_BACKWARD )
	{
		m_cameraVelocity.z -= 0.00001f;
	}

	if( _input == GG_ACTION_INPUT_MOVE_RIGHTWARD )
	{
		m_cameraVelocity.x += 0.00001f;
	}
	else if( _input == GG_ACTION_INPUT_MOVE_LEFTWARD )
	{
		m_cameraVelocity.x -= 0.00001f;
	}

	return;
}