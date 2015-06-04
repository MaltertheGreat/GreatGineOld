#pragma once

#include "GGChunk.h"

class GGPlayer
{
public:
	GGPlayer( GGChunk& _chunk );

public:
	void Update( GGChunk& _chunk, float _timeDelta );

private:
	GGChunk::GGObjectID m_headObjectID;
};