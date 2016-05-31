#pragma once

#include <map>
#include <set>
#include <array>
#include <utility>

#include "GGObject.h"
#include "GGObjectData.h"

#undef GetObject

class GGChunk
{
public:
	enum GG_CHUNK_STATE
	{
		GG_CHUNK_STATE_UNGENERATED,
		GG_CHUNK_STATE_GENERAETD
	};

	enum GG_OBJECT_IDS : int
	{
		GG_OBJECT_IDS_ADDED,
		GG_OBJECT_IDS_AWAKENED,
		GG_OBJECT_IDS_MODIFIED,
		GG_OBJECT_IDS_REMOVED,
		GG_OBJECT_IDS_EMPTY,

		GG_OBJECT_IDS_COUNT
	};

public:
	typedef UINT GGObjectID;
	typedef std::map<GGObjectID, GGObject> GGObjects;
	typedef std::set<GGObjectID> GGObjectIDList;

	typedef std::map<GGObjectData::GGObjectDataID, std::unique_ptr<GGObjectData>> GGObjectDataSet;
	typedef std::map<GGObjectID, GGObjectDataSet> GGObjectDataSets;

public:
	static constexpr float DIMENSION = 16.0f;

public:
	void Update();

	GGObjectID AddObject( GGObject&& _object );
	void ModifyObject( GGObjectID id, const DirectX::XMFLOAT3& _position );
	void RemoveObject( GGObjectID id );
	void ReplaceObject( GGObjectID id, GGObject&& _newObject );
	void AwakenObject( GGObjectID _id );

	void SetState( GG_CHUNK_STATE _state );

	template<typename GGObjectDataType>
	void SetObjectData( GGObjectID _objectID, std::unique_ptr<GGObjectData> _data );

	GG_CHUNK_STATE GetState() const;

	GGObject& GetObject( GGObjectID _id );
	const GGObjects& GetObjects() const;

	const GGObjectIDList& GetObjectIDList( GG_OBJECT_IDS _list ) const;

	template<typename GGObjectDataType>
	const GGObjectDataType GetObjectData( GGObjectID _objectID ) const;

private:
	GG_CHUNK_STATE    m_state = GG_CHUNK_STATE_UNGENERATED;

	GGObjects m_objects;
	std::array<GGObjectIDList, GG_OBJECT_IDS_COUNT> m_objectIDLists;
	std::array<GGObjectIDList, GG_OBJECT_IDS_COUNT> m_objectIDScheduledLists;

	GGObjectDataSets m_objectDataSets;
};

template<typename GGObjectDataType>
inline void GGChunk::SetObjectData( GGObjectID _objectID, std::unique_ptr<GGObjectData> _data )
{
	auto& objectDataSet = m_objectDataSets[_objectID];
	if( _data )
	{
		swap( objectDataSet[GGObjectDataType::ID], _data );
	}
	else
	{
		objectDataSet.erase( GGObjectDataType::ID );
	}

	return;
}

template<typename GGObjectDataType>
inline const GGObjectDataType GGChunk::GetObjectData( GGObjectID _objectID ) const
{
	auto objectDataSet = m_objectDataSets.find( _objectID );
	if( objectDataSet != m_objectDataSets.end() )
	{
		auto objectData = objectDataSet->second.find( GGObjectDataType::ID );

		if( objectData != objectDataSet->second.end() )
		{
			return *(dynamic_cast<GGObjectDataType*>(objectData->second.get()));
		}
	}

	return GGObjectDataType();
}
