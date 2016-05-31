#include "PCH.h"
#include "GGPhysics.h"
#include "GGPhysicsObjectData.h"
#include "GGWorld.h"

#include <vector>
#include <stack>
#include <random>

using namespace DirectX;
using namespace std;

void GGPhysics::Update( GGWorld& _world )
{
	for( auto& chunk : _world.GetChunkArray() )
	{
		auto& addedObjectIDs = chunk.GetObjectIDList( GGChunk::GG_OBJECT_IDS_ADDED );
		for( auto id : addedObjectIDs )
		{
			auto physicsData = chunk.GetObjectData<GGPhysicsObjectData>( id );

			if( physicsData.physical )
			{
				SplitObject( id, chunk );
			}
		}

		auto& awakenedObjectIDs = chunk.GetObjectIDList( GGChunk::GG_OBJECT_IDS_AWAKENED );
		for( auto id : awakenedObjectIDs )
		{
			auto physicsData = chunk.GetObjectData<GGPhysicsObjectData>( id );

			if( physicsData.physical )
			{
				UpdateObject( id, chunk );
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
		_chunk.RemoveObject( _objectID );
		vector<GGObject::GGVoxels> newVoxels( curLab - 1, GGObject::GGVoxels( GGObject::MAX_SIZE ) );

		for( UINT i = 0; i < newVoxels.size(); ++i )
		{
			for( UINT j = 0; j < GGObject::MAX_SIZE; ++j )
			{
				if( voxelLabels[j] == (i + 1) )
				{
					newVoxels[i][j] = voxels[j];
				}
			}

			static random_device generator;
			static uniform_real_distribution<float> distribution( 0.25f, 0.75f );
			XMFLOAT3 color = { distribution( generator ), distribution( generator ),  distribution( generator ) };

			GGObject newObject( move( newVoxels[i] ), object.GetVoxelDimension(), object.GetPosition(), color );

			_chunk.AddObject( move( newObject ) );
		}
	}

	return;
}

void GGPhysics::UpdateObject( GGChunk::GGObjectID& _objectID, GGChunk& _chunk )
{
	auto& object = _chunk.GetObject( _objectID );

	float objectFloorHeight = GetObjectFloorHeight( object );
	if( objectFloorHeight > 8.0f )
	{
		auto objectPos = object.GetPosition();
		objectPos.y -= 0.01f;
		object.SetPosition( objectPos );
		_chunk.ModifyObject( _objectID, objectPos );
	}
}

float GGPhysics::GetObjectFloorHeight( const GGObject& _object )
{
	float objectCenterHeight = _object.GetPosition().y;

	const UINT objectDiameter = GGObject::MAX_DIAMETER;
	auto& voxels = _object.GetVoxels();
	for( UINT y = 0; y < objectDiameter; ++y )
	{
		for( UINT x = 0; x < objectDiameter; ++x )
		{
			for( UINT z = 0; z < objectDiameter; ++z )
			{
				UINT i = x * objectDiameter * objectDiameter + y * objectDiameter + z;
				if( voxels[i].element != 0 )
				{
					int lowestVoxelLevel = y - (objectDiameter / 2);

					return objectCenterHeight + (lowestVoxelLevel * _object.GetVoxelDimension());
				}
			}
		}
	}

	return 0.0f;
}