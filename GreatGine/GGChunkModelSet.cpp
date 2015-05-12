#include "PCH.h"
#include "GGChunkModelSet.h"
#include "GGChunk.h"
#include "GGRenderer.h"

void GGChunkModelSet::Update( const GGDevice& _device, const GGChunk& _chunk )
{
	auto& newObjects = _chunk.GetNewObjects();
	auto position = _chunk.GetPosition();

	for( auto& object : newObjects )
	{
		m_models[ object.GetID() ] = GGObjectModel( _device, object, position );
	}

	return;
}

void GGChunkModelSet::Render( GGRenderer& _renderer ) const
{
	for( auto& it : m_models )
	{
		auto& model = it.second;

		_renderer.SetMesh( model.GetMesh() );
		_renderer.RenderMesh( model.GetMesh(), model.GetTransformation() );
	}

	return;
}