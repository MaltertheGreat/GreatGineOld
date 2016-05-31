#pragma once

#include  "GGChunk.h"

class GGWorld;
class GGObject;

class GGPhysics
{
public:
	void Update( GGWorld& _world );

private:
	void SplitObject( GGChunk::GGObjectID& _objectID, GGChunk& _chunk );
	void UpdateObject( GGChunk::GGObjectID& _objectID, GGChunk& _chunk );

	float GetObjectFloorHeight( const GGObject& _object );
};