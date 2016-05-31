#include "PCH.h"
#include "GGChunk.h"
using namespace DirectX;
using namespace std;

void GGChunk::Update()
{
	m_objectIDLists[GG_OBJECT_IDS_REMOVED].clear();
	for( auto id : m_objectIDScheduledLists[GG_OBJECT_IDS_REMOVED] )
	{
		if( m_objects.erase( id ) )
		{
			m_objectIDLists[GG_OBJECT_IDS_EMPTY].insert( id );
			m_objectIDLists[GG_OBJECT_IDS_REMOVED].insert( id );
			m_objectDataSets.erase( id );
		}
	}
	m_objectIDScheduledLists[GG_OBJECT_IDS_REMOVED].clear();

	swap( m_objectIDLists[GG_OBJECT_IDS_ADDED], m_objectIDScheduledLists[GG_OBJECT_IDS_ADDED] );
	m_objectIDScheduledLists[GG_OBJECT_IDS_ADDED].clear();

	swap( m_objectIDLists[GG_OBJECT_IDS_MODIFIED], m_objectIDScheduledLists[GG_OBJECT_IDS_MODIFIED] );
	m_objectIDScheduledLists[GG_OBJECT_IDS_MODIFIED].clear();

	swap( m_objectIDLists[GG_OBJECT_IDS_AWAKENED], m_objectIDScheduledLists[GG_OBJECT_IDS_AWAKENED] );
	m_objectIDScheduledLists[GG_OBJECT_IDS_AWAKENED].clear();

	return;
}

GGChunk::GGObjectID GGChunk::AddObject( GGObject&& _object )
{
	XMFLOAT3 objectPos = _object.GetPosition();
	assert( objectPos.x < (DIMENSION / 2.0f) && objectPos.x >= (DIMENSION / -2.0f) );
	assert( objectPos.z < (DIMENSION / 2.0f) && objectPos.z >= (DIMENSION / -2.0f) );

	std::pair<GGObjectID, GGObject> newObject( 0, move( _object ) );
	if( m_objectIDLists[GG_OBJECT_IDS_EMPTY].empty() )
	{
		newObject.first = m_objects.size();

		//Insert new object at the end
		m_objects.insert( m_objects.end(), move( newObject ) );
	}
	else
	{
		auto emptyObjectID = m_objectIDLists[GG_OBJECT_IDS_EMPTY].begin();
		newObject.first = *emptyObjectID;

		m_objectIDLists[GG_OBJECT_IDS_EMPTY].erase( emptyObjectID );

		// Insert new object in an empty spot
		m_objects.insert( move( newObject ) );
	}

	// Objects are added immediately, but the notification must occur the next frame
	m_objectIDScheduledLists[GG_OBJECT_IDS_ADDED].insert( newObject.first );
	m_objectIDScheduledLists[GG_OBJECT_IDS_AWAKENED].insert( newObject.first );

	return newObject.first;
}

void GGChunk::ModifyObject( GGObjectID _id, const DirectX::XMFLOAT3& _position )
{
	assert( _position.x < (DIMENSION / 2.0f) && _position.x >= (DIMENSION / -2.0f) );
	assert( _position.z < (DIMENSION / 2.0f) && _position.z >= (DIMENSION / -2.0f) );
	m_objects.at( _id ).SetPosition( _position );

	m_objectIDScheduledLists[GG_OBJECT_IDS_MODIFIED].insert( _id );
	m_objectIDScheduledLists[GG_OBJECT_IDS_AWAKENED].insert( _id );

	return;
}

void GGChunk::RemoveObject( GGObjectID _id )
{
	m_objectIDScheduledLists[GG_OBJECT_IDS_REMOVED].insert( _id );

	auto& toBeAddedObjectIDs = m_objectIDScheduledLists[GG_OBJECT_IDS_ADDED];
	auto it4 = find( toBeAddedObjectIDs.begin(), toBeAddedObjectIDs.end(), _id );
	if( it4 != toBeAddedObjectIDs.end() )
	{
		toBeAddedObjectIDs.erase( it4 );
	}

	auto& toBeModifiedObjectIDs = m_objectIDScheduledLists[GG_OBJECT_IDS_MODIFIED];
	auto it5 = find( toBeModifiedObjectIDs.begin(), toBeModifiedObjectIDs.end(), _id );
	if( it5 != toBeModifiedObjectIDs.end() )
	{
		toBeModifiedObjectIDs.erase( it5 );
	}

	auto& toBeAwakenedObjectIDs = m_objectIDScheduledLists[GG_OBJECT_IDS_AWAKENED];
	auto it6 = find( toBeAwakenedObjectIDs.begin(), toBeAwakenedObjectIDs.end(), _id );
	if( it6 != toBeAwakenedObjectIDs.end() )
	{
		toBeAwakenedObjectIDs.erase( it6 );
	}

	return;
}

void GGChunk::ReplaceObject( GGObjectID _id, GGObject&& _newObject )
{
	XMFLOAT3 objectPos = _newObject.GetPosition();
	assert( objectPos.x < (DIMENSION / 2.0f) && objectPos.x >= (DIMENSION / -2.0f) );
	assert( objectPos.z < (DIMENSION / 2.0f) && objectPos.z >= (DIMENSION / -2.0f) );

	auto& emptyObjectIDs = m_objectIDLists[GG_OBJECT_IDS_EMPTY];
	auto emptyObject = find( emptyObjectIDs.begin(), emptyObjectIDs.end(), _id );
	if( emptyObject != emptyObjectIDs.end() )
	{
		emptyObjectIDs.erase( emptyObject );
	}

	// Erase the current object
	m_objects.erase( _id );

	m_objects.insert( pair<GGObjectID, GGObject>( _id, move( _newObject ) ) );

	m_objectIDScheduledLists[GG_OBJECT_IDS_ADDED].insert( _id );
	m_objectIDScheduledLists[GG_OBJECT_IDS_AWAKENED].insert( _id );

	return;
}

void GGChunk::AwakenObject( GGObjectID _id )
{
	m_objectIDScheduledLists[GG_OBJECT_IDS_AWAKENED].insert( _id );

	return;
}

void GGChunk::SetState( GGChunk::GG_CHUNK_STATE _state )
{
	m_state = _state;

	return;
}

/*void GGChunk::SetObjectData( GGObjectID _objectID, std::unique_ptr<GGObjectData> _data )
{
	auto& objectDataSet = m_objectDataSets[_objectID];
	if( _data )
	{
		swap( objectDataSet[_data], _data );
	}
	else
	{
		objectDataSet.erase( _dataID );
	}

	return;
}*/

GGChunk::GG_CHUNK_STATE GGChunk::GetState() const
{
	return m_state;
}

GGObject& GGChunk::GetObject( GGObjectID _id )
{
	assert( m_objects.find( _id ) != m_objects.end() );

	return m_objects.at( _id );
}

const GGChunk::GGObjects& GGChunk::GetObjects() const
{
	return m_objects;
}

const GGChunk::GGObjectIDList& GGChunk::GetObjectIDList( GG_OBJECT_IDS _list ) const
{
	return m_objectIDLists[_list];
}