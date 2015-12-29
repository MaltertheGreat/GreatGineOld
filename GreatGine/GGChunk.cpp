#include "PCH.h"
#include "GGChunk.h"
using namespace DirectX;
using namespace std;

void GGChunk::Update()
{
	m_addedObjectIDs.clear();
	m_modifiedObjectIDs.clear();
	m_awakeObjectIDs.clear();

	return;
}

GGChunk::GGObjectID GGChunk::AddObject( GGObject&& _object )
{
	assert( !_object.IsEmpty() );
	XMFLOAT3 objectPos = _object.GetPosition();
	assert( objectPos.x < (DIMENSION / 2.0f) && objectPos.x >= (DIMENSION / -2.0f) );
	assert( objectPos.z < (DIMENSION / 2.0f) && objectPos.z >= (DIMENSION / -2.0f) );

	GGObjectID newObjectID;
	if( m_emptyObjectIDs.empty() )
	{
		newObjectID = m_objects.size();
		m_objects.push_back( move( _object ) );
	}
	else
	{
		newObjectID = m_emptyObjectIDs.back();
		m_emptyObjectIDs.pop_back();

		m_objects[ newObjectID ] = move( _object );
	}

	m_addedObjectIDs.push_back( newObjectID );
	m_awakeObjectIDs.push_back( newObjectID );

	return newObjectID;
}

void GGChunk::ModifyObject( GGObjectID _id, const DirectX::XMFLOAT3& _position )
{
	assert( _position.x < (DIMENSION / 2.0f) && _position.x >= (DIMENSION / -2.0f) );
	assert( _position.z < (DIMENSION / 2.0f) && _position.z >= (DIMENSION / -2.0f) );
	m_objects.at( _id ).SetPosition( _position );
	m_modifiedObjectIDs.push_back( _id );
	m_awakeObjectIDs.push_back( _id );

	return;
}

void GGChunk::RemoveObject( GGObjectID _id )
{
	if( !m_objects[ _id ].IsEmpty() )
	{
		m_objects[ _id ] = GGObject();

		m_addedObjectIDs.push_back( _id );
		m_emptyObjectIDs.push_back( _id );
	}

	return;
}

void GGChunk::ReplaceObject( GGObjectID _id, GGObject&& _newObject )
{
	assert( !_newObject.IsEmpty() );
	XMFLOAT3 objectPos = _newObject.GetPosition();
	assert( objectPos.x < (DIMENSION / 2.0f) && objectPos.x >= (DIMENSION / -2.0f) );
	assert( objectPos.z < (DIMENSION / 2.0f) && objectPos.z >= (DIMENSION / -2.0f) );

	auto emptyObject = find( m_emptyObjectIDs.begin(), m_emptyObjectIDs.end(), _id );
	if( emptyObject != m_emptyObjectIDs.end() )
	{
		m_emptyObjectIDs.erase( emptyObject );
	}

	m_objects[ _id ] = move( _newObject );

	m_addedObjectIDs.push_back( _id );
	m_awakeObjectIDs.push_back( _id );

	return;
}

void GGChunk::AwakenObject( GGObjectID _id )
{
	m_awakeObjectIDs.push_back( _id );

	return;
}

void GGChunk::SetState( GGChunk::GG_CHUNK_STATE _state )
{
	m_state = _state;

	return;
}

GGChunk::GG_CHUNK_STATE GGChunk::GetState() const
{
	return m_state;
}

const GGChunk::GGObjects& GGChunk::GetObjects() const
{
	return m_objects;
}

const GGChunk::GGObjectIDs& GGChunk::GetAddedObjectIDs() const
{
	return m_addedObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetModifiedObjectIDs() const
{
	return m_modifiedObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetAwakeObjectIDs() const
{
	return m_awakeObjectIDs;
}