#include "PCH.h"
#include "GGChunk.h"
using namespace DirectX;
using namespace std;

void GGChunk::Update()
{
	m_addedObjectIDs.clear();
	m_awakeObjectIDs.clear();
	m_modifiedObjectIDs.clear();
	m_removedObjectIDs.clear();

	return;
}

void GGChunk::SetState( GGChunk::GG_CHUNK_STATE _state )
{
	m_state = _state;

	return;
}

GGChunk::GGObjectID GGChunk::AddObject( GGObject&& _object )
{
	XMFLOAT3 objectPos = _object.GetPosition();
	assert( objectPos.x < (DIMENSION / 2.0f) && objectPos.x >= (DIMENSION / -2.0f) );
	assert( objectPos.z < (DIMENSION / 2.0f) && objectPos.z >= (DIMENSION / -2.0f) );

	GGObjectID newObjectID = 0;
	std::pair<GGObjectID, GGObject> newObject( 0, move( _object ) );
	if( m_emptyObjectIDs.empty() )
	{
		newObjectID = m_objects.size();
		newObject.first = newObjectID;

		//Insert new object at the end
		m_objects.insert( m_objects.end(), move( newObject ) );
	}
	else
	{
		newObjectID = m_emptyObjectIDs.back();
		newObject.first = newObjectID;

		m_emptyObjectIDs.pop_back();

		// Insert new object in an empty spot
		m_objects.insert( m_objects.end(), move( newObject ) );
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
	if( m_objects.erase( _id ) )
	{
		m_emptyObjectIDs.push_back( _id );
		m_removedObjectIDs.push_back( _id );
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

	// If key is already taken, erase the current object
	auto it = m_objects.find( _id );
	if( it != m_objects.end() )
	{
		m_objects.erase( it );
	}

	m_objects.insert( pair<GGObjectID, GGObject>( _id, move( _newObject ) ) );

	m_addedObjectIDs.push_back( _id );
	m_awakeObjectIDs.push_back( _id );

	return;
}

void GGChunk::AwakenObject( GGObjectID _id )
{
	m_awakeObjectIDs.push_back( _id );

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

const GGChunk::GGObjects& GGChunk::GetObjects() const
{
	return m_objects;
}

const GGChunk::GGObjectIDs& GGChunk::GetAddedObjectIDs() const
{
	return m_addedObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetAwakeObjectIDs() const
{
	return m_awakeObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetModifiedObjectIDs() const
{
	return m_modifiedObjectIDs;
}

const GGChunk::GGObjectIDs& GGChunk::GetRemovedObjectIDs() const
{
	return m_removedObjectIDs;
}