#pragma once

#include <unordered_map>
#include <vector>

#include "GGObject.h"

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
	typedef std::unordered_map<GGObjectID, GGObject> GGObjects;
	typedef std::vector<GGObjectID> GGObjectIDs;

public:
	static constexpr float DIMENSION = 16.0f;

public:
	GGChunk();
	explicit GGChunk( DirectX::XMFLOAT3 _position );

public:
	void Update();

	GGObjectID AddObject( GGObject&& _object );
	void ModifyObject( GGObjectID id, const DirectX::XMFLOAT3& _position );
	void ReplaceObject( GGObjectID id, GGObject&& _newObject );
	void RemoveObject( GGObjectID id );

	void SetState( GG_CHUNK_STATE _state );

	GG_CHUNK_STATE GetState() const;
	const DirectX::XMFLOAT3& GetPosition() const;

	const GGObjects& GetObjects() const;
	const GGObjectIDs& GetAddedObjectIDs() const;
	const GGObjectIDs& GetModifiedObjectIDs() const;
	const GGObjectIDs& GetRemovedObjectIDs() const;
	// TODO:
	//const GGObjectIDs& GetMovedObjectIDs() const;

private:
	GG_CHUNK_STATE    m_state = GG_CHUNK_STATE_UNGENERATED;
	DirectX::XMFLOAT3 m_position; // Relative to center of loaded world

	GGObjects   m_objects;
	GGObjectID  m_newObjectID = 0;
	GGObjectIDs m_addedObjectIDs;
	GGObjectIDs m_modifiedObjectIDs;
	GGObjectIDs m_removedObjectIDs;
};