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
	m_removedObjectIDs.clear();

	return;
}

GGChunk::GGObjectID GGChunk::AddObject( GGObject&& _object )
{
	m_objects.insert( { m_newObjectID, move( _object ) } ).second;

	m_addedObjectIDs.push_back( m_newObjectID );

	return m_newObjectID++;
}

void GGChunk::ModifyObject( GGObjectID id, const DirectX::XMFLOAT3& _position )
{
	m_objects.at( id ).SetPosition( _position );
	m_modifiedObjectIDs.push_back( id );

	return;
}

void GGChunk::ReplaceObject( GGObjectID id, GGObject && _newObject )
{
	m_objects.at( id ) = move( _newObject );
	m_addedObjectIDs.push_back( id );

	return;
}

void GGChunk::RemoveObject( GGObjectID id )
{
	m_objects.erase( id );
	m_removedObjectIDs.push_back( id );

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

const GGChunk::GGObjectIDs& GGChunk::GetRemovedObjectIDs() const
{
	return m_removedObjectIDs;
}