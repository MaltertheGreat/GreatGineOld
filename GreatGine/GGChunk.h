#pragma once

#include <map>
#include <vector>
#include <utility>

#include "GGObject.h"
#include "GGObjectData.h"

class GGChunk
{
public:
	enum GG_CHUNK_STATE
	{
		GG_CHUNK_STATE_UNGENERATED,
		GG_CHUNK_STATE_GENERAETD
	};

public:
	typedef UINT GGObjectID;
	typedef std::map<GGObjectID, GGObject> GGObjects;
	typedef std::vector<GGObjectID> GGObjectIDs;

	typedef std::map<GGObjectData::GGObjectDataID, std::unique_ptr<GGObjectData>> GGObjectDataSet;
	typedef std::map<GGObjectID, GGObjectDataSet> GGObjectDataSets;

public:
	static constexpr float DIMENSION = 16.0f;

public:
	void Update();

	void SetState( GG_CHUNK_STATE _state );

	GGObjectID AddObject( GGObject&& _object );
	void ModifyObject( GGObjectID id, const DirectX::XMFLOAT3& _position );
	void RemoveObject( GGObjectID id );
	void ReplaceObject( GGObjectID id, GGObject&& _newObject );
	void AwakenObject( GGObjectID _id );

	void SetObjectData( GGObjectID _objectID, GGObjectData::GGObjectDataID _dataID, std::unique_ptr<GGObjectData> _data );

	GG_CHUNK_STATE GetState() const;

	const GGObjects& GetObjects() const;

	const GGObjectIDs& GetAddedObjectIDs() const;
	const GGObjectIDs& GetAwakenedObjectIDs() const;
	const GGObjectIDs& GetModifiedObjectIDs() const;
	const GGObjectIDs& GetRemovedObjectIDs() const;

	template<typename T>
	const T* GetObjectData( GGObjectID _objectID, GGObjectData::GGObjectDataID _dataID ) const;

private:
	GG_CHUNK_STATE    m_state = GG_CHUNK_STATE_UNGENERATED;

	GGObjects m_objects;
	GGObjectIDs m_emptyObjectIDs;

	GGObjectIDs m_addedObjectIDs;
	GGObjectIDs m_modifiedObjectIDs;
	GGObjectIDs m_removedObjectIDs;
	GGObjectIDs m_awakenedObjectIDs;

	GGObjectDataSets m_objectDataSets;
};

template<typename T>
inline const T* GGChunk::GetObjectData( GGObjectID _objectID, GGObjectData::GGObjectDataID _dataID ) const
{
	try
	{
		auto& objectDataSet = m_objectDataSets.at( _objectID );
		auto& objectData = objectDataSet.at( _dataID );

		assert( objectData );
		if( objectData->GetID() == _dataID )
		{
			return dynamic_cast<T*>(objectData.get());
		}
	}
	catch( ... )
	{
		return nullptr;
	}

	return nullptr;
}