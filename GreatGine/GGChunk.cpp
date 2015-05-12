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
	// Mark new objects as no-longer new
	m_objects.splice( m_objects.end(), move( m_newObjects ) );

	return;
}

void GGChunk::AddObject( GGObject&& _object )
{
	m_newObjects.push_front( move( _object ) );

	return;
}

void GGChunk::SetState( GGChunk::GG_CHUNK_STATE _state )
{
	m_state = _state;

	return;
}

UINT GGChunk::GetNextObjectID()
{
	return m_objectID++;
}

GGChunk::GG_CHUNK_STATE GGChunk::GetState() const
{
	return m_state;
}

const DirectX::XMFLOAT3& GGChunk::GetPosition() const
{
	return m_position;
}

const GGChunk::GGObjectList& GGChunk::GetObjects() const
{
	return m_objects;
}

const GGChunk::GGObjectList& GGChunk::GetNewObjects() const
{
	return m_newObjects;
}