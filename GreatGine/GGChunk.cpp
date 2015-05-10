#include "PCH.h"
#include "GGChunk.h"
using namespace std;

void GGChunk::AddObject( GGObject && _object )
{
	m_objects.push_back( move( _object ) );

	return;
}

const std::vector<GGObject>& GGChunk::GetObjects() const
{
	return m_objects;
}