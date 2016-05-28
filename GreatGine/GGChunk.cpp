#include "PCH.h"
#include "GGChunk.h"
using namespace DirectX;
using namespace std;

void GGChunk::Update()
{
	m_removedObjectIDs.clear();
	for( auto id : m_toBeRemovedObjectIDs )
	{
		if( m_objects.erase( id ) )
		{
			m_emptyObjectIDs.insert( id );
			m_removedObjectIDs.insert( id );
		}
	}
	m_toBeRemovedObjectIDs.clear();

	swap( m_addedObjectIDs, m_toBeAddedObjectIDs );
	m_toBeAddedObjectIDs.clear();

	swap( m_modifiedObjectIDs, m_toBeModifiedObjectIDs );
	m_toBeModifiedObjectIDs.clear();

	swap( m_awakenedObjectIDs, m_toBeAwakenedObjectIDs );
	m_toBeAwakenedObjectIDs.clear();

	return;
}

GGChunk::GGObjectID GGChunk::AddObject( GGObject&& _object )
{
	XMFLOAT3 objectPos = _object.GetPosition();
	assert( objectPos.x < (DIMENSION / 2.0f) && objectPos.x >= (DIMENSION / -2.0f) );
	assert( objectPos.z < (DIMENSION / 2.0f) && objectPos.z >= (DIMENSION / -2.0f) );

	std::pair<GGObjectID, GGObject> newObject( 0, move( _object ) );
	if( m_emptyObjectIDs.empty() )
	{
		newObject.first = m_objects.size();

		//Insert new object at the end
		m_objects.insert( m_objects.end(), move( newObject ) );
	}
	else
	{
		auto emptyObjectID = m_emptyObjectIDs.begin();
		newObject.first = *emptyObjectID;

		m_emptyObjectIDs.erase( emptyObjectID );

		// Insert new object in an empty spot
		m_objects.insert( move( newObject ) );
	}

	// Objects are added immediately, but the notification must occur the next frame
	m_toBeAddedObjectIDs.insert( newObject.first );
	m_toBeAwakenedObjectIDs.insert( newObject.first );

	return newObject.first;
}

void GGChunk::ModifyObject( GGObjectID _id, const DirectX::XMFLOAT3& _position )
{
	assert( _position.x < (DIMENSION / 2.0f) && _position.x >= (DIMENSION / -2.0f) );
	assert( _position.z < (DIMENSION / 2.0f) && _position.z >= (DIMENSION / -2.0f) );
	m_objects.at( _id ).SetPosition( _position );

	m_toBeModifiedObjectIDs.insert( _id );
	m_toBeAwakenedObjectIDs.insert( _id );

	return;
}

void GGChunk::RemoveObject( GGObjectID _id )
{
	m_toBeRemovedObjectIDs.insert( _id );

	auto it1 = find( m_addedObjectIDs.begin(), m_addedObjectIDs.end(), _id );
	if( it1 != m_addedObjectIDs.end() )
	{
		m_addedObjectIDs.erase( it1 );
	}

	auto it2 = find( m_modifiedObjectIDs.begin(), m_modifiedObjectIDs.end(), _id );
	if( it2 != m_modifiedObjectIDs.end() )
	{
		m_modifiedObjectIDs.erase( it2 );
	}

	auto it3 = find( m_awakenedObjectIDs.begin(), m_awakenedObjectIDs.end(), _id );
	if( it3 != m_awakenedObjectIDs.end() )
	{
		m_awakenedObjectIDs.erase( it3 );
	}

	auto it4 = find( m_toBeAddedObjectIDs.begin(), m_toBeAddedObjectIDs.end(), _id );
	if( it4 != m_toBeAddedObjectIDs.end() )
	{
		m_toBeAddedObjectIDs.erase( it4 );
	}
	auto it5 = find( m_toBeModifiedObjectIDs.begin(), m_toBeModifiedObjectIDs.end(), _id );
	if( it5 != m_toBeModifiedObjectIDs.end() )
	{
		m_toBeModifiedObjectIDs.erase( it5 );
	}

	auto it6 = find( m_toBeAwakenedObjectIDs.begin(), m_toBeAwakenedObjectIDs.end(), _id );
	if( it6 != m_toBeAwakenedObjectIDs.end() )
	{
		m_toBeAwakenedObjectIDs.erase( it6 );
	}

	return;
}

void GGChunk::ReplaceObject( GGObjectID _id, GGObject&& _newObject )
{
	XMFLOAT3 objectPos = _newObject.GetPosition();
	assert( objectPos.x < (DIMENSION / 2.0f) && objectPos.x >= (DIMENSION / -2.0f) );
	assert( objectPos.z < (DIMENSION / 2.0f) && objectPos.z >= (DIMENSION / -2.0f) );

	auto emptyObject = find( m_emptyObjectIDs.begin(), m_emptyObjectIDs.end(), _id );
	if( emptyObject != m_emptyObjectIDs.end() )
	{
		m_emptyObjectIDs.erase( emptyObject );
	}

	// Erase the current object
	m_objects.erase( _id );

	m_objects.insert( pair<GGObjectID, GGObject>( _id, move( _newObject ) ) );

	m_toBeAddedObjectIDs.insert( _id );
	m_toBeAwakenedObjectIDs.insert( _id );

	return;
}

void GGChunk::AwakenObject( GGObjectID _id )
{
	m_toBeAwakenedObjectIDs.insert( _id );

	return;
}

void GGChunk::SetState( GGChunk::GG_CHUNK_STATE _state )
{
	m_state = _state;

	return;
}

void GGChunk::SetObjectData( GGObjectID _objectID, GGObjectData::GGObjectDataID _dataID, std::unique_ptr<GGObjectData> _data )
{
	auto& objectDataSet = m_objectDataSets[_objectID];
	if( _data )
	{
		swap( objectDataSet[_dataID], _data );
	}
	else
	{
		objectDataSet.erase( _dataID );
	}

	return;
}

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

const GGChunk::GGObjectIDs& GGChunk::GetAddedObjectIDs() const
{
	return m_addedObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetAwakenedObjectIDs() const
{
	return m_awakenedObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetModifiedObjectIDs() const
{
	return m_modifiedObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetRemovedObjectIDs() const
{
	return m_removedObjectIDs;
}