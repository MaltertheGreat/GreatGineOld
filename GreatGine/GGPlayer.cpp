#include "PCH.h"
#include "GGPlayer.h"
#include "GGInput.h"
#include "GGConfig.h"

#include <cmath>
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
	if( !m_isAlive )
	{
		m_position = { 0.0f, 10.0f, 0.0f };
		m_chunkX = GGWorld::DIAMETER / 2;
		m_chunkZ = GGWorld::DIAMETER / 2;

		auto& chunk = _world.GetChunk( m_chunkX, m_chunkZ );
		if( chunk.GetState() == GGChunk::GG_CHUNK_STATE_UNGENERATED )
		{
			return;
		}

		m_headObjectID = chunk.AddObject( PlayerObject( m_position ) );

		m_isAlive = true;
	}

	UpdatePosition( _world, _timeDelta );
	InteractWithWorld( _world, _timeDelta );

	return;
}

const XMFLOAT3 GGPlayer::GetPosition() const
{
	XMFLOAT3 position; // Relative to center of the world
	float offset = -((GGWorld::DIAMETER / 2) * GGChunk::DIMENSION) + (GGChunk::DIMENSION / 2);
	position = { offset, 0, offset };
	position.x += m_chunkX * GGChunk::DIMENSION;
	position.z += m_chunkZ * GGChunk::DIMENSION;

	position.x += m_position.x;
	position.y += m_position.y;
	position.z += m_position.z;

	return position;
}

const XMFLOAT3& GGPlayer::GetRotation() const
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

GGObject GGPlayer::PlayerObject( const DirectX::XMFLOAT3& _pos )
{
	GGObject::GGVoxelArray voxels;
	voxels[ 0 ].element = 1;
	//voxels[ 2152 ].element = 1;
	voxels[ 4095 ].element = 1;
	float bodyRadius = 1.0f;

	return GGObject( move( voxels ), bodyRadius, _pos );
}

void GGPlayer::UpdatePosition( GGWorld & _world, float _timeDelta )
{
	auto& head = _world.GetChunk( m_chunkX, m_chunkZ ).GetObjects().at( m_headObjectID );

	// Update player position since last frame
	XMFLOAT3 horizontalRotation = { 0.0f, m_rotation.y, 0.0f };
	XMVECTOR rotation = XMLoadFloat3( &horizontalRotation );
	XMVECTOR velocity = XMLoadFloat3( &m_velocity );
	XMVECTOR position = XMLoadFloat3( &head.GetPosition() );

	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );
	velocity = XMVector3Rotate( velocity, rotation );
	position += XMVectorScale( velocity, _timeDelta );

	int chunkOffsetX = 0;
	int chunkOffsetZ = 0;

	// Keep player in chunk boundaries
	float x = XMVectorGetX( position );
	while( x > (GGChunk::DIMENSION / 2.0f) )
	{
		x -= GGChunk::DIMENSION;
		++chunkOffsetX;
	}
	while( x <= -(GGChunk::DIMENSION / 2.0f) )
	{
		x += GGChunk::DIMENSION;
		--chunkOffsetX;
	}

	float y = XMVectorGetY( position );

	float z = XMVectorGetZ( position );
	while( z > (GGChunk::DIMENSION / 2.0f) )
	{
		z -= GGChunk::DIMENSION;
		++chunkOffsetZ;
	}
	while( z <= -(GGChunk::DIMENSION / 2.0f) )
	{
		z += GGChunk::DIMENSION;
		--chunkOffsetZ;
	}

	// Keep player in world boundaries
	UINT chunkX = m_chunkX;
	if( chunkOffsetX )
	{
		if( -chunkOffsetX > static_cast<LONGLONG>(chunkX) )
		{
			chunkX = 0;
		}
		else if( chunkOffsetX > static_cast<LONGLONG>(GGWorld::DIAMETER - 1 - chunkX) )
		{
			chunkX = GGWorld::DIAMETER - 1;
		}
		else
		{
			chunkX += chunkOffsetX;
		}
	}

	UINT chunkZ = m_chunkZ;
	if( chunkOffsetZ )
	{
		if( -chunkOffsetZ > static_cast<LONGLONG>(chunkZ) )
		{
			chunkZ = 0;
		}
		else if( chunkOffsetZ > static_cast<LONGLONG>(GGWorld::DIAMETER - 1 - chunkZ) )
		{
			chunkZ = GGWorld::DIAMETER - 1;
		}
		else
		{
			chunkZ += chunkOffsetZ;
		}
	}

	// Update player model
	if( m_chunkX != chunkX || m_chunkZ != chunkZ )
	{
		_world.GetChunk( m_chunkX, m_chunkZ ).RemoveObject( m_headObjectID );

		m_chunkX = chunkX;
		m_chunkZ = chunkZ;

		m_headObjectID = _world.GetChunk( m_chunkX, m_chunkZ ).AddObject( PlayerObject( { x, y, z } ) );
	}
	else
	{
		_world.GetChunk( m_chunkX, m_chunkZ ).ModifyObject( m_headObjectID, { x, y, z } );
	}

	XMStoreFloat3( &m_position, position );

	return;
}

int sgn( float f )
{
	if( f > 0.0f )
	{
		return 1;
	}
	else if( f < 0.0f )
	{
		return -1;
	}
	else
	{
		return 0;
	}
}

void GGPlayer::InteractWithWorld( GGWorld & _world, float _timeDelta )
{
	XMVECTOR ray = XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 1.0f ), XMQuaternionRotationRollPitchYaw( m_rotation.x, m_rotation.y, m_rotation.z ) );
	float rayX = XMVectorGetX( ray );
	float rayY = XMVectorGetY( ray );
	float rayZ = XMVectorGetZ( ray );

	auto& objects = _world.GetChunk( m_chunkX, m_chunkZ ).GetObjects();
	for( auto iterator : objects )
	{
		GGChunk::GGObjectID objectID = iterator.first;
		if( objectID == m_headObjectID )
		{
			continue;
		}

		auto& object = iterator.second;
		float objectRadius = GGObject::DIAMETER / 2.0f;
		XMVECTOR relativePos = XMLoadFloat3( &m_position ) - XMLoadFloat3( &object.GetPosition() ); // Relative player position to the center of object

		XMVECTOR objectBounds = XMVectorReplicate( objectRadius );
		if( XMVector3InBounds( relativePos, objectBounds ) )
		{
			// First voxel
			UINT x = static_cast<UINT>(XMVectorGetX( relativePos ) + objectRadius);
			UINT y = static_cast<UINT>(XMVectorGetY( relativePos ) + objectRadius);
			UINT z = static_cast<UINT>(XMVectorGetZ( relativePos ) + objectRadius);

			float voxelDiameter = 1.0f;
			// Ray origin inside voxel
			float voxelX = fmod( XMVectorGetX( relativePos ), voxelDiameter );
			float voxelY = fmod( XMVectorGetY( relativePos ), voxelDiameter );
			float voxelZ = fmod( XMVectorGetZ( relativePos ), voxelDiameter );

			// Length of the ray to hit voxel boundary
			float lengthX;
			if( rayX > 0 )
			{
				lengthX = (voxelDiameter - voxelX) / rayX;
			}
			else if( rayX < 0 )
			{
				lengthX = (0.0f - voxelX) / rayX;
			}
			else
			{
				lengthX = INFINITY;
			}
			float lengthY;
			if( rayY > 0 )
			{
				lengthY = (voxelDiameter - voxelY) / rayY;
			}
			else if( rayY < 0 )
			{
				lengthY = (0.0f - voxelY) / rayY;
			}
			else
			{
				lengthY = INFINITY;
			}
			float lengthZ;
			if( rayZ > 0 )
			{
				lengthZ = (voxelDiameter - voxelZ) / rayZ;
			}
			else if( rayZ < 0 )
			{
				lengthZ = (0.0f - voxelZ) / rayZ;
			}
			else
			{
				lengthZ = INFINITY;
			}

			if( lengthX < lengthY )
			{
				if( lengthX < lengthZ )
				{
					x += sgn( rayX );
					if( x > GGObject::DIAMETER * GGObject::DIAMETER * GGObject::DIAMETER )
					{
						continue;
					}
				}
			}
			
			if( lengthY < lengthX )
			{
				if( lengthY < lengthZ )
				{
					y += sgn( rayY );
					if( y > GGObject::DIAMETER * GGObject::DIAMETER * GGObject::DIAMETER )
					{
						continue;
					}
				}
			}

			if( lengthZ < lengthX )
			{
				if( lengthZ < lengthY )
				{
					z += sgn( rayZ );
					if( z > GGObject::DIAMETER * GGObject::DIAMETER * GGObject::DIAMETER )
					{
						continue;
					}
				}
			}


			UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
			auto& voxels = object.GetVoxels();
			if( voxels.at( voxelIndex ).element != 0 )
			{
				GGObject::GGVoxelArray newVoxels = voxels;
				newVoxels.at( voxelIndex ).element = 0;

				GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), object.GetPosition() );

				_world.GetChunk( m_chunkX, m_chunkZ ).ReplaceObject( objectID, move( newObject ) );
			}
		}
	}

	return;
}