#pragma once

#include <list>

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
	typedef std::list<GGObject> GGObjectList;

public:
	static constexpr float DIMENSION = 16.0f;

public:
	GGChunk();
	explicit GGChunk( DirectX::XMFLOAT3 _position );

public:
	void Update();

	void AddObject( GGObject&& _object );

	void SetState( GG_CHUNK_STATE _state );

	UINT GetNextObjectID();
	GG_CHUNK_STATE GetState() const;
	const DirectX::XMFLOAT3& GetPosition() const;

	const GGObjectList& GetObjects() const;
	const GGObjectList& GetNewObjects() const;

private:
	UINT m_objectID = 0;
	GG_CHUNK_STATE    m_state = GG_CHUNK_STATE_UNGENERATED;
	DirectX::XMFLOAT3 m_position; // Relative to center of loaded world

	GGObjectList m_objects;
	GGObjectList m_newObjects;
};