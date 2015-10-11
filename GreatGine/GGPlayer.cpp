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
		m_position = GGConvertToGG( { 0.0f, 10.0f, 0.0f } );
		m_chunkX = GGWorld::DIAMETER / 2;
		m_chunkZ = GGWorld::DIAMETER / 2;

		auto& chunk = _world.GetChunk( m_chunkX, m_chunkZ );
		if( chunk.GetState() == GGChunk::GG_CHUNK_STATE_UNGENERATED )
		{
			return;
		}

		m_headObjectID = chunk.AddObject( PlayerObject( GGConvertToSI( m_position ) ) );

		m_isAlive = true;
	}

	UpdatePosition( _world );
	InteractWithWorld( _world );

	return;
}

const XMFLOAT3 GGPlayer::GetPosition() const
{
	XMFLOAT3 position; // Relative to center of the world
	float offset = -((GGWorld::DIAMETER / 2) * GGChunk::DIMENSION) + (GGChunk::DIMENSION / 2);
	position = { offset, 0, offset };
	position.x += m_chunkX * GGChunk::DIMENSION;
	position.z += m_chunkZ * GGChunk::DIMENSION;

	position.x += GGConvertToSI( m_position.x );
	position.y += GGConvertToSI( m_position.y );
	position.z += GGConvertToSI( m_position.z );

	return position;
}

const XMFLOAT3& GGPlayer::GetRotation() const
{
	return m_rotation;
}

void GGPlayer::HandleKeyInput( WPARAM _keyCode, bool _down )
{
	int velocity;
	bool digging;
	bool placing;
	if( _down )
	{
		velocity = 2;
		digging = true;
		placing = true;
	}
	else
	{
		velocity = GGConvertToGG( 0.0f );
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
	const float piDiv2 = 1.57079631f;// PI/2 rounded down for float precision-thingy
	m_rotation.x += _y * 0.005f;
	if( m_rotation.x >= piDiv2 )
	{
		m_rotation.x = piDiv2;
	}
	else if( m_rotation.x <= -piDiv2 )
	{
		m_rotation.x = -piDiv2;
	}

	m_rotation.y += _x * 0.005f;
	if( m_rotation.y >= XMConvertToRadians( 360.0f ) )
	{
		m_rotation.y -= XMConvertToRadians( 360.0f );
	}
	else if( m_rotation.y <= 0 )
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

void GGPlayer::UpdatePosition( GGWorld & _world )
{
	XMVECTOR position = XMLoadSInt3( &m_position );
	XMVECTOR velocity = XMLoadSInt3( &m_velocity );

	XMFLOAT3 horizontalRotation = { 0.0f, m_rotation.y, 0.0f };
	XMVECTOR rotation = XMLoadFloat3( &horizontalRotation );
	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );

	velocity = XMVector3Rotate( velocity, rotation );

	position += velocity;

	XMStoreSInt3( &m_position, position );

	_world.GetChunk( m_chunkX, m_chunkZ ).ModifyObject( m_headObjectID, GGConvertToSI( m_position ) );

	return;
}

void GGPlayer::InteractWithWorld( GGWorld & _world )
{
	const UINT cooldown = 6;
	if( m_diggingCooldown > 0 )
	{
		--m_diggingCooldown;
	}

	if( m_placingCooldown > 0 )
	{
		--m_placingCooldown;
	}

	if( !m_digging && !m_placing )
	{
		return;
	}

	if( m_diggingCooldown == 0 || m_placingCooldown == 0 )
	{
		auto voxelObjectChunk = _world.GetVoxelFromRay( m_chunkX, m_chunkZ, GGConvertToSI( m_position ), m_rotation, 5.0f, &m_headObjectID );

		if( voxelObjectChunk )
		{
			auto& chunk = _world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ );
			auto& object = chunk.GetObjects().at( voxelObjectChunk->objectID );
			UINT x = voxelObjectChunk->voxelX;
			UINT y = voxelObjectChunk->voxelY;
			UINT z = voxelObjectChunk->voxelZ;
			GGVoxel::GG_VOXEL_FACE face = voxelObjectChunk->face;


			if( m_digging && m_diggingCooldown == 0 )
			{
				UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;

				GGObject::GGVoxelArray newVoxels = object.GetVoxels();
				newVoxels.at( voxelIndex ).element = 0;

				GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), object.GetPosition() );

				_world.GetChunk( voxelObjectChunk->chunkX, voxelObjectChunk->chunkZ ).ReplaceObject( voxelObjectChunk->objectID, move( newObject ) );

				m_diggingCooldown = cooldown;
			}

			if( m_placing && m_placingCooldown == 0 )
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