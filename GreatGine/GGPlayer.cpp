#include "PCH.h"
#include "GGPlayer.h"
#include "GGInput.h"
#include "GGConfig.h"
#include "GGPhysicsObjectData.h"

#include <random>
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
		m_headObject.chunk.chunkX = GGWorld::DIAMETER / 2;
		m_headObject.chunk.chunkZ = GGWorld::DIAMETER / 2;

		auto& chunk = _world.GetChunk( m_headObject.chunk );

		if( chunk.GetState() == GGChunk::GG_CHUNK_STATE_UNGENERATED )
		{
			return;
		}

		m_headObject.objectID = chunk.AddObject( PlayerObject( m_position ) );

		m_isAlive = true;
	}

	auto& chunk = _world.GetChunk( m_headObject.chunk );

	GGObjectData::GGObjectDataID id = GGPhysicsObjectData::magicID;
	unique_ptr<GGPhysicsObjectData> objectData = make_unique<GGPhysicsObjectData>( id, false, m_velocity );
	chunk.SetObjectData( m_headObject.objectID, id, move( objectData ) );

	UpdatePosition( _world, _timeDelta );
	InteractWithWorld( _world, _timeDelta );

	return;
}

const XMFLOAT3 GGPlayer::GetPosition() const
{
	return m_position;
}

const XMFLOAT3& GGPlayer::GetRotation() const
{
	return m_rotation;
}

const GGWorld::GGChunkDescription GGPlayer::GetChunk() const
{
	return m_headObject.chunk;
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

	if( _keyCode == m_keyMap[GG_KEYMAP_FORWARD] )
	{
		m_velocity.z = velocity;
	}
	else if( _keyCode == m_keyMap[GG_KEYMAP_BACKWARD] )
	{
		m_velocity.z = -velocity;
	}

	if( _keyCode == m_keyMap[GG_KEYMAP_RIGHTWARD] )
	{
		m_velocity.x = velocity;
	}
	else if( _keyCode == m_keyMap[GG_KEYMAP_LEFTWARD] )
	{
		m_velocity.x = -velocity;
	}

	if( _keyCode == m_keyMap[GG_KEYMAP_UPWARD] )
	{
		m_velocity.y = velocity;
	}
	else if( _keyCode == m_keyMap[GG_KEYMAP_DOWNWARD] )
	{
		m_velocity.y = -velocity;
	}

	if( _keyCode == m_keyMap[GG_KEYMAP_DIG] )
	{
		m_digging = digging;
	}

	if( _keyCode == m_keyMap[GG_KEYMAP_PLACE] )
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

GGObject GGPlayer::PlayerObject( const XMFLOAT3& _pos )
{
	GGObject::GGVoxels voxels( GGObject::MAX_SIZE );
	voxels[0].element = 1;
	//voxels[ 2152 ].element = 1;
	voxels[4095].element = 1;
	float bodyRadius = 1.0f;
	XMFLOAT3 bodyColor = { 1.0f, 0.0f, 1.0f };

	return GGObject( move( voxels ), bodyRadius, _pos, bodyColor );
}

void GGPlayer::UpdatePosition( GGWorld& _world, double _timeDelta )
{
	auto& chunk = _world.GetChunk( m_headObject.chunk );
	auto& head = chunk.GetObjects().at( m_headObject.objectID );

	GGObjectData::GGObjectDataID id = GGPhysicsObjectData::magicID;
	auto playerData = chunk.GetObjectData<GGPhysicsObjectData>( m_headObject.objectID, id );

	// Update player position since last frame
	XMFLOAT3 horizontalRotation = { 0.0f, m_rotation.y, 0.0f };
	XMVECTOR rotation = XMLoadFloat3( &horizontalRotation );
	XMVECTOR velocity = XMLoadFloat3( &playerData->velocity );
	XMVECTOR position = XMLoadFloat3( &head.GetPosition() );

	rotation = XMQuaternionRotationRollPitchYawFromVector( rotation );
	velocity = XMVector3Rotate( velocity, rotation );
	position += XMVectorScale( velocity, static_cast<float>(_timeDelta) );

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
	UINT chunkX = m_headObject.chunk.chunkX;
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

	UINT chunkZ = m_headObject.chunk.chunkZ;
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
	if( m_headObject.chunk.chunkX != chunkX || m_headObject.chunk.chunkZ != chunkZ )
	{
		_world.GetChunk( m_headObject.chunk ).RemoveObject( m_headObject.objectID );

		m_headObject.chunk.chunkX = chunkX;
		m_headObject.chunk.chunkZ = chunkZ;

		m_headObject.objectID = _world.GetChunk( m_headObject.chunk ).AddObject( PlayerObject( { x, y, z } ) );
	}
	else
	{
		_world.GetChunk( m_headObject.chunk ).ModifyObject( m_headObject.objectID, { x, y, z } );
	}

	m_position = { x, y, z };

	return;
}

void GGPlayer::InteractWithWorld( GGWorld& _world, double _timeDelta )
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

	if( m_placing && m_placingCooldown == 0.0 )
	{
		auto voxelObjectChunk = _world.GetVoxelFromRay( m_headObject.chunk.chunkX, m_headObject.chunk.chunkZ, m_position, m_rotation, 5.0f, &m_headObject );

		if( voxelObjectChunk )
		{
			PlaceVoxel( *voxelObjectChunk, _world );

			m_placingCooldown = cooldown;
		}
	}

	if( m_digging && m_diggingCooldown == 0.0f )
	{
		auto voxelObjectChunk = _world.GetVoxelFromRay( m_headObject.chunk.chunkX, m_headObject.chunk.chunkZ, m_position, m_rotation, 5.0f, &m_headObject );

		if( voxelObjectChunk )
		{
			auto& chunk = _world.GetChunk( voxelObjectChunk->voxel.object.chunk );
			auto& object = chunk.GetObjects().at( voxelObjectChunk->voxel.object.objectID );
			UINT x = voxelObjectChunk->voxel.voxelX;
			UINT y = voxelObjectChunk->voxel.voxelY;
			UINT z = voxelObjectChunk->voxel.voxelZ;

			UINT voxelIndex = x * GGObject::MAX_DIAMETER * GGObject::MAX_DIAMETER + y * GGObject::MAX_DIAMETER + z;

			GGObject::GGVoxels newVoxels = object.GetVoxels();
			newVoxels.at( voxelIndex ).element = 0;

			bool objectEmpty = true;
			for( auto voxel : newVoxels )
			{
				if( voxel.element != 0 )
				{
					objectEmpty = false;
					break;
				}
			}

			if( objectEmpty )
			{
				chunk.RemoveObject( voxelObjectChunk->voxel.object.objectID );
			}
			else
			{
				GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), object.GetPosition(), object.GetColor() );

				chunk.ReplaceObject( voxelObjectChunk->voxel.object.objectID, move( newObject ) );
			}

			m_diggingCooldown = cooldown;
		}
	}

	return;
}

void GGPlayer::PlaceVoxel( const GGWorld::GGVoxelFaceDescription& _desc, GGWorld& _world )
{
	auto& chunk = _world.GetChunk( _desc.voxel.object.chunk );
	auto& object = chunk.GetObjects().at( _desc.voxel.object.objectID );
	UINT x = _desc.voxel.voxelX;
	UINT y = _desc.voxel.voxelY;
	UINT z = _desc.voxel.voxelZ;
	GGVoxel::GG_VOXEL_FACE face = _desc.face;

	bool createNewObject = false;
	XMFLOAT3 newObjectPos = object.GetPosition();
	GGWorld::GGChunkDescription newObjectChunk = _desc.voxel.object.chunk;

	switch( face )
	{
		case GGVoxel::GG_VOXEL_FACE_TOP:
		{
			if( y < (GGObject::MAX_DIAMETER - 1) )
			{
				++y;
			}
			else
			{
				createNewObject = true;
				y = 0;
				newObjectPos.y += GGObject::MAX_DIAMETER * object.GetVoxelDimension();
			}

			break;
		}
		case GGVoxel::GG_VOXEL_FACE_BOTTOM:
		{
			if( y > 0 )
			{
				--y;
			}
			else
			{
				createNewObject = true;
				y = GGObject::MAX_DIAMETER - 1;
				newObjectPos.y -= GGObject::MAX_DIAMETER * object.GetVoxelDimension();
			}

			break;
		}
		case GGVoxel::GG_VOXEL_FACE_NORTH:
		{
			if( z < (GGObject::MAX_DIAMETER - 1) )
			{
				++z;
			}
			else
			{
				createNewObject = true;
				z = 0;
				newObjectPos.z += GGObject::MAX_DIAMETER * object.GetVoxelDimension();
				if( newObjectPos.z >= (GGChunk::DIMENSION / 2.0f) )
				{
					newObjectPos.z -= GGChunk::DIMENSION;
					newObjectChunk.chunkZ++;
				}
			}

			break;
		}
		case GGVoxel::GG_VOXEL_FACE_SOUTH:
		{
			if( z > 0 )
			{
				--z;
			}
			else
			{
				createNewObject = true;
				z = GGObject::MAX_DIAMETER - 1;
				newObjectPos.z -= GGObject::MAX_DIAMETER * object.GetVoxelDimension();
				if( newObjectPos.z < (GGChunk::DIMENSION / -2.0f) )
				{
					newObjectPos.z += GGChunk::DIMENSION;
					newObjectChunk.chunkZ--;
				}
			}

			break;
		}
		case GGVoxel::GG_VOXEL_FACE_EAST:
		{
			if( x < (GGObject::MAX_DIAMETER - 1) )
			{
				++x;
			}
			else
			{
				createNewObject = true;
				x = 0;
				newObjectPos.x += GGObject::MAX_DIAMETER * object.GetVoxelDimension();
				if( newObjectPos.x >= (GGChunk::DIMENSION / 2.0f) )
				{
					newObjectPos.x -= GGChunk::DIMENSION;
					newObjectChunk.chunkX++;
				}
			}

			break;
		}
		case GGVoxel::GG_VOXEL_FACE_WEST:
		{
			if( x > 0 )
			{
				--x;
			}
			else
			{
				createNewObject = true;
				x = GGObject::MAX_DIAMETER - 1;
				newObjectPos.x -= GGObject::MAX_DIAMETER * object.GetVoxelDimension();
				if( newObjectPos.x < (GGChunk::DIMENSION / -2.0f) )
				{
					newObjectPos.x += GGChunk::DIMENSION;
					newObjectChunk.chunkX--;
				}
			}

			break;
		}
	}

	UINT voxelIndex = x * GGObject::MAX_DIAMETER * GGObject::MAX_DIAMETER + y * GGObject::MAX_DIAMETER + z;

	if( createNewObject )
	{
		static random_device generator;
		static uniform_real_distribution<float> distribution( 0.25f, 0.75f );

		GGObject::GGVoxels newVoxels( GGObject::MAX_SIZE );
		newVoxels.at( voxelIndex ).element = 1;

		XMFLOAT3 color = { distribution( generator ), distribution( generator ),  distribution( generator ) };

		GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newObjectPos, color );

		_world.GetChunk( newObjectChunk ).AddObject( move( newObject ) );
	}
	else
	{
		GGObject::GGVoxels newVoxels = object.GetVoxels();
		newVoxels.at( voxelIndex ).element = 1;

		GGObject newObject = GGObject( move( newVoxels ), object.GetVoxelDimension(), newObjectPos, object.GetColor() );

		_world.GetChunk( newObjectChunk ).ReplaceObject( _desc.voxel.object.objectID, move( newObject ) );
	}

	return;
}