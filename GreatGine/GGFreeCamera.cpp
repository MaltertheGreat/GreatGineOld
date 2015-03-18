#include "PCH.h"
#include "GGFreeCamera.h"
using namespace DirectX;

GGFreeCamera::GGFreeCamera( const XMFLOAT3& _position )
	:
	m_position( _position ),
	m_velocity( { 0.0f, 0.0f, 0.0f } ),
	m_rotation( { 0.0f, 0.0f, 0.0f } )
{}

void GGFreeCamera::Update( float _frameTime )
{
	XMVECTOR position = XMLoadFloat3( &m_position );
	XMVECTOR velocity = XMLoadFloat3( &m_velocity );
	XMVECTOR rotation = XMLoadFloat3( &m_rotation );

	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );
	velocity = XMVector3Rotate( velocity, rotation );
	position = XMVectorAdd( position, XMVectorScale( velocity, _frameTime ) );

	XMStoreFloat3( &m_position, position );

	return;
}

void GGFreeCamera::HandleActionInput( GG_ACTION_INPUT _input, bool _down )
{
	if( _down )
	{
		if( _input == GG_ACTION_INPUT_MOVE_FORWARD )
		{
			m_velocity.z = 5.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_BACKWARD )
		{
			m_velocity.z = -5.0f;
		}

		if( _input == GG_ACTION_INPUT_MOVE_RIGHTWARD )
		{
			m_velocity.x = 5.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_LEFTWARD )
		{
			m_velocity.x = -5.0f;
		}
	}
	else
	{
		if( _input == GG_ACTION_INPUT_MOVE_FORWARD )
		{
			m_velocity.z = 0.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_BACKWARD )
		{
			m_velocity.z = 0.0f;
		}

		if( _input == GG_ACTION_INPUT_MOVE_RIGHTWARD )
		{
			m_velocity.x = 0.0f;
		}
		else if( _input == GG_ACTION_INPUT_MOVE_LEFTWARD )
		{
			m_velocity.x = 0.0f;
		}
	}

	return;
}

void GGFreeCamera::HandleRangeInput( int _x, int _y )
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