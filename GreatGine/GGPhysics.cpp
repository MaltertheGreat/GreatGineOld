#include "PCH.h"
#include "GGPhysics.h"
#include "GGWorld.h"

using namespace DirectX;

void GGPhysics::Update( GGWorld& _world )
{
	for( auto& chunk : _world.GetChunkArray() )
	{
		auto& awakenedObjectIDs = chunk.GetAwakenedObjectIDs();
		for( auto id : awakenedObjectIDs )
		{
			if( id == 5 )
			{
				// Just for testing purposes
				auto& object = chunk.GetObject( id );
				XMFLOAT3 position = object.GetPosition();
				position.y += 0.00001f;
				chunk.ModifyObject( id, position );
			}
		}
	}
}