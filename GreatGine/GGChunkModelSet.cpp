#include "PCH.h"
#include "GGChunkModelSet.h"
#include "GGChunk.h"
#include "GGRenderer.h"

void GGChunkModelSet::Update( const GGDevice& _device, const GGChunk& _chunk )
{
	auto& objects = _chunk.GetObjects();
	auto position = _chunk.GetPosition();

	auto& newObjectIDs = _chunk.GetAddedObjectIDs();
	for( auto& id : newObjectIDs )
	{
		m_models[ id ] = GGObjectModel( _device, objects.at( id ), position );
	}

	auto& modifiedObjectIDs = _chunk.GetModifiedObjectIDs();
	for( auto& id : modifiedObjectIDs )
	{
		m_models.at( id ).Update( objects.at( id ), position );
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