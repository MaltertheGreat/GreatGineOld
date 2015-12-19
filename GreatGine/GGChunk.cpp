#include "PCH.h"
#include "GGChunk.h"
using namespace std;

GGChunk::GGChunk()
{}

GGChunk::GGChunk( DirectX::XMFLOAT3 _position )
	:
	m_position( _position )
{}

void GGChunk::Update()
{
	m_addedObjectIDs.clear();
	m_modifiedObjectIDs.clear();

	return;
}

GGChunk::GGObjectID GGChunk::AddObject( GGObject&& _object )
{
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

	return newObjectID;
}

void GGChunk::ModifyObject( GGObjectID _id, const DirectX::XMFLOAT3& _position )
{
	m_objects.at( _id ).SetPosition( _position );
	m_modifiedObjectIDs.push_back( _id );

	return;
}

void GGChunk::RemoveObject( GGObjectID _id )
{
	ReplaceObject( _id, GGObject() );
	m_emptyObjectIDs.push_back( _id );

	return;
}

void GGChunk::ReplaceObject( GGObjectID _id, GGObject&& _newObject )
{
	if( _newObject.IsEmpty() )
	{
		m_emptyObjectIDs.push_back( _id );
	}

	m_objects.at( _id ) = move( _newObject );

	m_addedObjectIDs.push_back( _id );

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

const DirectX::XMFLOAT3& GGChunk::GetPosition() const
{
	return m_position;
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