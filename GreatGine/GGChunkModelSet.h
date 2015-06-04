#pragma once

#include <unordered_map>

#include "GGObjectModel.h"

class GGChunk;
class GGDevice;
class GGRenderer;

class GGChunkModelSet
{
public:
	typedef unsigned UINT;
	typedef std::unordered_map< UINT, GGObjectModel> GGObjectModels;

public:
	void Update( const GGDevice& _device, const GGChunk& _chunk );
	void Render( GGRenderer& _renderer ) const;

private:
	GGObjectModels m_models;
};