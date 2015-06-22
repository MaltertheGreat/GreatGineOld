#include "PCH.h"
#include "GGPlayer.h"
#include "GGInput.h"
#include "GGConfig.h"
using namespace std;
using namespace DirectX;

GGPlayer::GGPlayer( GGInput& _input, GGConfig& _config )
	:
	m_isAlive( false ),
	m_keyMap{
	_config.GetUint( "key_forward", 'W' ),
	_config.GetUint( "key_backward", 'S' ),
	_config.GetUint( "key_rightward", 'D' ),
	_config.GetUint( "key_leftward", 'A' ),
	_config.GetUint( "key_upward", VK_SPACE ),
	_config.GetUint( "key_downward", VK_SHIFT ) }
{
	_input.RegisterHandler( this );
}

void GGPlayer::Update( GGWorld& _world, float _timeDelta )
{
	const UINT chunkIndex = (GGWorld::DIAMETER / 2 * GGWorld::DIAMETER) + GGWorld::DIAMETER / 2;
	auto& chunk = _world.GetChunkArray().at( chunkIndex );
	if( chunk.GetState() == GGChunk::GG_CHUNK_STATE_UNGENERATED )
	{
		return;
	}

	if( !m_isAlive )
	{
		GGObject::GGVoxelArray voxels;
		voxels[ 0 ].element = 1;
		float bodyRadius = 1.0f;
		XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
		GGObject playerHead = GGObject( move( voxels ), bodyRadius, position );

		m_headObjectID = chunk.AddObject( move( playerHead ) );

		m_isAlive = true;
	}
	else
	{
		auto& head = chunk.GetObjects().at( m_headObjectID );

		XMFLOAT3 horizontalRotation = { 0.0f, m_rotation.y, 0.0f };
		XMVECTOR rotation = XMLoadFloat3( &horizontalRotation );
		XMVECTOR velocity = XMLoadFloat3( &m_velocity );
		XMVECTOR position = XMLoadFloat3( &head.GetPosition() );

		rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );
		velocity = XMVector3Rotate( velocity, rotation );
		position += XMVectorScale( velocity, _timeDelta );

		XMStoreFloat3( &m_position, position );
		chunk.ModifyObject( m_headObjectID, m_position );
	}

	return;
}

const DirectX::XMFLOAT3 & GGPlayer::GetPosition() const
{
	return m_position;
}

const DirectX::XMFLOAT3 & GGPlayer::GetRotation() const
{
	return m_rotation;
}

void GGPlayer::HandleKeyInput( WPARAM _keyCode, bool _down )
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

void GGPlayer::HandleMouseInput( int _x, int _y )
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