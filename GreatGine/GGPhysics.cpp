#include "PCH.h"
#include "GGPhysics.h"
#include "GGPhysicsObjectData.h"
#include "GGWorld.h"

#include <vector>
#include <stack>

using namespace DirectX;
using namespace std;

void GGPhysics::Update( GGWorld& _world )
{
	for( auto& chunk : _world.GetChunkArray() )
	{
		auto& modifiedObjectIDs = chunk.GetAddedObjectIDs();
		for( auto id : modifiedObjectIDs )
		{
			auto physicsData = chunk.GetObjectData<GGPhysicsObjectData>( id, GGPhysicsObjectData::magicID );

			if( (!physicsData) || (physicsData && physicsData->physical) )
			{
				SplitObject( id, chunk );
			}
		}
	}

	return;
}

void GGPhysics::SplitObject( GGChunk::GGObjectID& _objectID, GGChunk& _chunk )
{
	GGObject& object = _chunk.GetObject( _objectID );
	const GGObject::GGVoxels& voxels = object.GetVoxels();
	UINT objectDiameter = object.MAX_DIAMETER;

	vector<BYTE> voxelLabels( object.MAX_SIZE );
	BYTE curLab = 1;
	stack<UINT> voxelsToCheck;

	for( UINT i = 0; i < voxelLabels.size(); ++i )
	{
		if( (voxels[i].element != 0) && (voxelLabels[i] == 0) )
		{
			voxelLabels[i] = curLab;
			voxelsToCheck.push( i );

			while( !voxelsToCheck.empty() )
			{
				UINT j = voxelsToCheck.top();
				voxelsToCheck.pop();

				UINT x = j / (objectDiameter * objectDiameter);
				UINT y = (j / objectDiameter) % objectDiameter;
				UINT z = j % objectDiameter;

				// Right
				if( x < (objectDiameter - 1) )
				{
					UINT k = (x + 1) * objectDiameter * objectDiameter + y * objectDiameter + z;
					if( (voxels[k].element != 0) && (voxelLabels[k] == 0) )
					{
						voxelLabels[k] = curLab;
						voxelsToCheck.push( k );
					}
				}

				// Left
				if( x > 0 )
				{
					UINT k = (x - 1) * objectDiameter * objectDiameter + y * objectDiameter + z;
					if( (voxels[k].element != 0) && (voxelLabels[k] == 0) )
					{
						voxelLabels[k] = curLab;
						voxelsToCheck.push( k );
					}
				}

				// Top
				if( y < (objectDiameter - 1) )
				{
					UINT k = x * objectDiameter * objectDiameter + (y + 1) * objectDiameter + z;
					if( (voxels[k].element != 0) && (voxelLabels[k] == 0) )
					{
						voxelLabels[k] = curLab;
						voxelsToCheck.push( k );
					}
				}

				// Bottom
				if( y > 0 )
				{
					UINT k = x * objectDiameter * objectDiameter + (y - 1) * objectDiameter + z;
					if( (voxels[k].element != 0) && (voxelLabels[k] == 0) )
					{
						voxelLabels[k] = curLab;
						voxelsToCheck.push( k );
					}
				}

				// Front
				if( z < (objectDiameter - 1) )
				{
					UINT k = x * objectDiameter * objectDiameter + y * objectDiameter + (z + 1);
					if( (voxels[k].element != 0) && (voxelLabels[k] == 0) )
					{
						voxelLabels[k] = curLab;
						voxelsToCheck.push( k );
					}
				}

				// Back
				if( z > 0 )
				{
					UINT k = x * objectDiameter * objectDiameter + y * objectDiameter + (z - 1);
					if( (voxels[k].element != 0) && (voxelLabels[k] == 0) )
					{
						voxelLabels[k] = curLab;
						voxelsToCheck.push( k );
					}
				}
			}

			++curLab;
		}
	}

	if( curLab > 2 )
	{
		auto pos = object.GetPosition();
		pos.y += 0.1f;
		object.SetPosition( pos );

		_chunk.ModifyObject( _objectID, pos );
	}
}