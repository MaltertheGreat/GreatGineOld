#include "PCH.h"
#include "GGPlayer.h"
#include "GGInput.h"
#include "GGConfig.h"

using namespace std;
using namespace DirectX;

GGPlayer::GGPlayer( GGInput& _input, GGConfig& _config )
	:
	m_isAlive( false ),
	m_digging( false ),
	m_placing( false ),
	m_keyMap{
	_config.GetUint( "key_forward", 'W' ),
	_config.GetUint( "key_backward", 'S' ),
	_config.GetUint( "key_rightward", 'D' ),
	_config.GetUint( "key_leftward", 'A' ),
	_config.GetUint( "key_upward", VK_SPACE ),
	_config.GetUint( "key_downward", VK_SHIFT ),
	_config.GetUint( "key_dig", VK_LBUTTON ),
	_config.GetUint( "key_place", VK_RBUTTON ) }
{
	_input.RegisterHandler( this );
}

void GGPlayer::Update( GGWorld& _world, double _timeDelta )
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
	float velocity;
	bool digging;
	bool placing;
	if( _down )
	{
		velocity = 5.0f;
		digging = true;
		placing = true;
	}
	else
	{
		velocity = 0.0f;
		digging = false;
		placing = false;
	}

	if( _keyCode == m_keyMap[ GG_KEYMAP_FORWARD ] )
	{
		m_velocity.z = velocity;
	}
	else if( _keyCode == m_keyMap[ GG_KEYMAP_BACKWARD ] )
	{
		m_velocity.z = -velocity;
	}

	if( _keyCode == m_keyMap[ GG_KEYMAP_RIGHTWARD ] )
	{
		m_velocity.x = velocity;
	}
	else if( _keyCode == m_keyMap[ GG_KEYMAP_LEFTWARD ] )
	{
		m_velocity.x = -velocity;
	}

	if( _keyCode == m_keyMap[ GG_KEYMAP_UPWARD ] )
	{
		m_velocity.y = velocity;
	}
	else if( _keyCode == m_keyMap[ GG_KEYMAP_DOWNWARD ] )
	{
		m_velocity.y = -velocity;
	}

	if( _keyCode == m_keyMap[ GG_KEYMAP_DIG ] )
	{
		m_digging = digging;
	}

	if( _keyCode == m_keyMap[ GG_KEYMAP_PLACE ] )
	{
		m_placing = placing;
	}

	return;
}

void GGPlayer::HandleMouseInput( int _x, int _y )
{
	m_rotation.x += _y * 0.005f;
	if( m_rotation.x >= XM_PIDIV2 )
	{
		m_rotation.x = XM_PIDIV2;
	}
	else if( m_rotation.x <= -XM_PIDIV2 )
	{
		m_rotation.x = -XM_PIDIV2;
	}

	m_rotation.y += _x * 0.005f;
	if( m_rotation.y >= XM_PI )
	{
		m_rotation.y = m_rotation.y - XM_2PI;
	}
	else if( m_rotation.y < -XM_PI )
	{
		m_rotation.y = XM_2PI + m_rotation.y;
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

void GGPlayer::UpdatePosition( GGWorld & _world, double _timeDelta )
{
	auto& head = _world.GetChunk( m_chunkX, m_chunkZ ).GetObjects().at( m_headObjectID );

	// Update player position since last frame
	XMFLOAT3 horizontalRotation = { 0.0f, m_rotation.y, 0.0f };
	XMVECTOR rotation = XMLoadFloat3( &horizontalRotation );
	XMVECTOR velocity = XMLoadFloat3( &m_velocity );
	XMVECTOR position = XMLoadFloat3( &head.GetPosition() );

	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );
	velocity = XMVector3Rotate( velocity, rotation );
	position += XMVectorScale( velocity, static_cast<float>(_timeDelta) );
	//position += XMVectorSet( 1.0f, 0.0f, 0.0f, 0.0f );

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

	m_position = { x, y, z };

	return;
}

void GGPlayer::InteractWithWorld( GGWorld & _world, double _timeDelta )
{
	const float cooldown = 0.1f;
	if( m_diggingCooldown > static_cast<float>(_timeDelta) )
	{
		m_diggingCooldown -= static_cast<float>(_timeDelta);
	}
	else
	{
		m_diggingCooldown = 0.0f;
	}

	if( m_placingCooldown > static_cast<float>(_timeDelta) )
	{
		m_placingCooldown -= static_cast<float>(_timeDelta);
	}
	else
	{
		m_placingCooldown = 0.0f;
	}

	if( !m_digging && !m_placing )
	{
		return;
	}

	if( m_diggingCooldown == 0.0f || m_placingCooldown == 0.0f )
	{
		auto voxelObjectChunk = _world.GetVoxelFromRay( m_chunkX, m_chunkZ, m_position, m_rotation, 5.0f, &m_headObjectID );

		if( voxelObjectChunk )
		{
			auto& chunk = _world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ );
			auto& object = chunk.GetObjects().at( voxelObjectChunk->objectID );
			UINT x = voxelObjectChunk->voxelX;
			UINT y = voxelObjectChunk->voxelY;
			UINT z = voxelObjectChunk->voxelZ;
			GGVoxel::GG_VOXEL_FACE face = voxelObjectChunk->face;

			if( m_digging && m_diggingCooldown == 0.0f )
			{
				UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;

				GGObject::GGVoxelArray newVoxels = object.GetVoxels();
				newVoxels.at( voxelIndex ).element = 0;

				GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), object.GetPosition() );

				_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).ReplaceObject( voxelObjectChunk->objectID, move( newObject ) );

				m_diggingCooldown = cooldown;
			}

			if( m_placing && m_placingCooldown == 0.0 )
			{
				switch( face )
				{
					case GGVoxel::GG_VOXEL_FACE_TOP:
					{
						if( y < (GGObject::DIAMETER - 1) )
						{
							++y;

							break;
						}
						else
						{
							y = 0;

							GGObject::GGVoxelArray newVoxels;
							UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
							newVoxels.at( voxelIndex ).element = 1;

							XMFLOAT3 newPos = object.GetPosition();
							newPos.y += GGObject::DIAMETER * object.GetVoxelDimension();

							GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newPos );

							_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).AddObject( move( newObject ) );

							m_placingCooldown = cooldown;

							return;
						}
					}
					case GGVoxel::GG_VOXEL_FACE_BOTTOM:
					{
						if( y > 0 )
						{
							--y;

							break;
						}
						else
						{
							y = GGObject::DIAMETER - 1;

							GGObject::GGVoxelArray newVoxels;
							UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
							newVoxels.at( voxelIndex ).element = 1;

							XMFLOAT3 newPos = object.GetPosition();
							newPos.y -= GGObject::DIAMETER * object.GetVoxelDimension();

							GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newPos );

							_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).AddObject( move( newObject ) );

							m_placingCooldown = cooldown;

							return;
						}
					}
					case GGVoxel::GG_VOXEL_FACE_NORTH:
					{
						if( z < (GGObject::DIAMETER - 1) )
						{
							++z;

							break;
						}
						else
						{
							z = 0;

							GGObject::GGVoxelArray newVoxels;
							UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
							newVoxels.at( voxelIndex ).element = 1;

							XMFLOAT3 newPos = object.GetPosition();
							newPos.z += GGObject::DIAMETER * object.GetVoxelDimension();

							GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newPos );

							_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).AddObject( move( newObject ) );

							m_placingCooldown = cooldown;

							return;
						}
					}
					case GGVoxel::GG_VOXEL_FACE_SOUTH:
					{
						if( z > 0 )
						{
							--z;

							break;
						}
						else
						{
							z = GGObject::DIAMETER - 1;

							GGObject::GGVoxelArray newVoxels;
							UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
							newVoxels.at( voxelIndex ).element = 1;

							XMFLOAT3 newPos = object.GetPosition();
							newPos.z -= GGObject::DIAMETER * object.GetVoxelDimension();

							GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newPos );

							_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).AddObject( move( newObject ) );

							m_placingCooldown = cooldown;

							return;
						}
					}
					case GGVoxel::GG_VOXEL_FACE_EAST:
					{
						if( x < (GGObject::DIAMETER - 1) )
						{
							++x;

							break;
						}
						else
						{
							x = 0;

							GGObject::GGVoxelArray newVoxels;
							UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
							newVoxels.at( voxelIndex ).element = 1;

							XMFLOAT3 newPos = object.GetPosition();
							newPos.x += GGObject::DIAMETER * object.GetVoxelDimension();

							GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newPos );

							_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).AddObject( move( newObject ) );

							m_placingCooldown = cooldown;

							return;
						}
					}
					case GGVoxel::GG_VOXEL_FACE_WEST:
					{
						if( x > 0 )
						{
							--x;

							break;
						}
						else
						{
							x = GGObject::DIAMETER - 1;

							GGObject::GGVoxelArray newVoxels;
							UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
							newVoxels.at( voxelIndex ).element = 1;

							XMFLOAT3 newPos = object.GetPosition();
							newPos.x -= GGObject::DIAMETER * object.GetVoxelDimension();

							GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newPos );

							_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).AddObject( move( newObject ) );

							m_placingCooldown = cooldown;

							return;
						}
					}
				}

				UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;

				GGObject::GGVoxelArray newVoxels = object.GetVoxels();
				newVoxels.at( voxelIndex ).element = 1;

				GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), object.GetPosition() );

				_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).ReplaceObject( voxelObjectChunk->objectID, move( newObject ) );

				m_placingCooldown = cooldown;
			}
		}
	}

	return;
}