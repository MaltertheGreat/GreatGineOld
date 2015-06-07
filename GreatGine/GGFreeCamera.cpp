#include "PCH.h"
#include "GGFreeCamera.h"
#include "GGConfig.h"
using namespace DirectX;

GGFreeCamera::GGFreeCamera( const XMFLOAT3& _position, GGConfig& _config )
	:
	m_keyMap{
	_config.GetUint( "key_forward", 'W' ),
	_config.GetUint( "key_backward", 'S' ),
	_config.GetUint( "key_rightward", 'D' ),
	_config.GetUint( "key_leftward", 'A' ),
	_config.GetUint( "key_upward", VK_SPACE ),
	_config.GetUint( "key_downward", VK_SHIFT ),
},
m_position( _position ),
m_velocity( { 0.0f, 0.0f, 0.0f } ),
m_rotation( { 0.0f, 0.0f, 0.0f } )
{}

void GGFreeCamera::Update( float _frameTime )
{
	XMVECTOR position = XMLoadFloat3( &m_position );
	XMVECTOR velocity = XMLoadFloat3( &m_velocity );
	XMFLOAT3 horizontalRotation = { 0.0f, m_rotation.y, 0.0f };
	XMVECTOR rotation = XMLoadFloat3( &horizontalRotation );

	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );
	velocity = XMVector3Rotate( velocity, rotation );
	position = XMVectorAdd( position, XMVectorScale( velocity, _frameTime ) );

	XMStoreFloat3( &m_position, position );

	return;
}

void GGFreeCamera::HandleKeyInput( WPARAM _keyCode, bool _down )
{
	if( _down )
	{
		if( _keyCode == m_keyMap[ GG_KEYMAP_FORWARD ] )
		{
			m_velocity.z = 5.0f;
		}
		else if( _keyCode == m_keyMap[ GG_KEYMAP_BACKWARD ] )
		{
			m_velocity.z = -5.0f;
		}

		if( _keyCode == m_keyMap[ GG_KEYMAP_RIGHTWARD ] )
		{
			m_velocity.x = 5.0f;
		}
		else if( _keyCode == m_keyMap[ GG_KEYMAP_LEFTWARD ] )
		{
			m_velocity.x = -5.0f;
		}

		if( _keyCode == m_keyMap[ GG_KEYMAP_UPWARD ] )
		{
			m_velocity.y = 5.0f;
		}
		else if( _keyCode == m_keyMap[ GG_KEYMAP_DOWNWARD ] )
		{
			m_velocity.y = -5.0f;
		}
	}
	else
	{
		if( _keyCode == m_keyMap[ GG_KEYMAP_FORWARD ] )
		{
			m_velocity.z = 0.0f;
		}
		else if( _keyCode == m_keyMap[ GG_KEYMAP_BACKWARD ] )
		{
			m_velocity.z = 0.0f;
		}

		if( _keyCode == m_keyMap[ GG_KEYMAP_RIGHTWARD ] )
		{
			m_velocity.x = 0.0f;
		}
		else if( _keyCode == m_keyMap[ GG_KEYMAP_LEFTWARD ] )
		{
			m_velocity.x = 0.0f;
		}

		if( _keyCode == m_keyMap[ GG_KEYMAP_UPWARD ] )
		{
			m_velocity.y = 0.0f;
		}
		else if( _keyCode == m_keyMap[ GG_KEYMAP_DOWNWARD ] )
		{
			m_velocity.y = 0.0f;
		}
	}

	return;
}

void GGFreeCamera::HandleMouseInput( int _x, int _y )
{
	m_rotation.x += _y * 0.005f;
	if( m_rotation.x >= XMConvertToRadians( 89.9999f ) )
	{
		m_rotation.x = XMConvertToRadians( 89.9999f );
	}
	else if( m_rotation.x <= XMConvertToRadians( -89.9999f ) )
	{
		m_rotation.x = XMConvertToRadians( -89.9999f );
	}

	m_rotation.y += _x * 0.005f;
	if( m_rotation.y > XMConvertToRadians( 360.0f ) )
	{
		m_rotation.y -= XMConvertToRadians( 360.0f );
	}
	else if( m_rotation.y < 0 )
	{
		m_rotation.y += XMConvertToRadians( 360.0f );
	}

	return;
}

const XMFLOAT3& GGFreeCamera::GetPosition() const
{
	return m_position;
}

const XMFLOAT3& GGFreeCamera::GetRotation() const
{
	return m_rotation;
}