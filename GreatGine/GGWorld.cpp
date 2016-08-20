#include "PCH.h"
#include "GGWorld.h"

#include <random>
#include <string>
using namespace DirectX;
using namespace std;

void GGWorld::Update()
{
	for( auto& chunk : m_chunks )
	{
		if( chunk.GetState() == GGChunk::GG_CHUNK_STATE_UNGENERATED )
		{
			GenerateChunk( chunk );
		}

		chunk.Update();
	}

	return;
}

void GGWorld::SetViewPoint( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation )
{
	m_viewPointPosition = _position;
	m_viewPointRotation = _rotation;

	return;
}

GGChunk& GGWorld::GetChunk( GGChunkDescription _desc )
{
	UINT index = _desc.chunkX * DIAMETER + _desc.chunkZ;

	return m_chunks[index];
}

GGWorld::GGChunkArray& GGWorld::GetChunkArray()
{
	return m_chunks;
}

const GGChunk& GGWorld::GetChunk( GGChunkDescription _desc ) const
{
	UINT index = _desc.chunkX * DIAMETER + _desc.chunkZ;

	return m_chunks[index];
}

const GGWorld::GGChunkArray& GGWorld::GetChunkArray() const
{
	return m_chunks;
}

const DirectX::XMFLOAT3& GGWorld::GetViewPointPosition() const
{
	return m_viewPointPosition;
}

const DirectX::XMFLOAT3& GGWorld::GetViewPointRotation() const
{
	return m_viewPointRotation;
}

unique_ptr<GGWorld::GGVoxelFaceDescription> GGWorld::GetVoxelFromRay( UINT _originChunkX, UINT _originChunkZ, const XMFLOAT3& _originPosition, const XMFLOAT3& _rotation, float _length, GGObjectDescription* _excludedObject )
{
	XMFLOAT3 ray;
	XMStoreFloat3( &ray, XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 1.0f ), XMQuaternionRotationRollPitchYaw( _rotation.x, _rotation.y, _rotation.z ) ) );

	float minRayLength = INFINITY;
	unique_ptr<GGVoxelFaceDescription> closestVoxelDesc;

	auto chunksToCheck = ChunksToCheckRay( _originChunkX, _originChunkZ, _originPosition );

	for( auto& chunkIterator : chunksToCheck )
	{
		GGChunkDescription chunk = chunkIterator.first;
		const auto& objects = GetChunk( chunk ).GetObjects();

		for( auto& object : objects )
		{
			if( _excludedObject )
			{
				if( chunk == _excludedObject->chunk )
				{
					if( object.first == _excludedObject->objectID )
					{
						continue;
					}
				}
			}

			auto voxelFromRay = GetVoxelFromRayInObject( chunkIterator.second, ray, _length, object.second );

			float rayLength = voxelFromRay.second;

			if( rayLength < minRayLength )
			{
				minRayLength = rayLength;
				closestVoxelDesc = move( voxelFromRay.first );

				closestVoxelDesc->voxel.object.objectID = object.first;
				closestVoxelDesc->voxel.object.chunk = chunk;
			}
		}
	}

	return move( closestVoxelDesc );
}

void GGWorld::PlaceVoxelOn( GGVoxelFaceDescription _voxelFace )
{
	auto& chunk = GetChunk( _voxelFace.voxel.object.chunk );
	auto& object = chunk.GetObject( _voxelFace.voxel.object.objectID );
	XMUINT3 objectDiameter = object.GetDiameter();
	XMFLOAT3 objectPosition = object.GetPosition();
	XMFLOAT3 objectColor = object.GetColor();
	float voxelDimension = object.GetVoxelDimension();
	float voxelRadius = voxelDimension / 2.0f;
	XMUINT3 voxelCoords =
	{
		_voxelFace.voxel.voxelX,
		_voxelFace.voxel.voxelY,
		_voxelFace.voxel.voxelZ
	};

	bool createNewObject = false;
	GGObject::GGVoxels voxels;
	static random_device generator;
	static uniform_real_distribution<float> distribution( 0.25f, 0.75f );

	switch( _voxelFace.face )
	{
		case GGVoxel::GG_VOXEL_FACE_EAST:
			if( voxelCoords.x == (objectDiameter.x - 1) )
			{
				if( objectDiameter.x < GGObject::MAX_DIAMETER )
				{
					XMUINT3 oldObjectDiameter = objectDiameter;
					++objectDiameter.x;
					objectPosition.x += voxelRadius;

					voxels.resize( objectDiameter.x * objectDiameter.y * objectDiameter.z );

					auto& oldVoxels = object.GetVoxels();
					for( UINT x = 0; x < (objectDiameter.x - 1); ++x )
					{
						for( UINT y = 0; y < objectDiameter.y; ++y )
						{
							for( UINT z = 0; z < objectDiameter.z; ++z )
							{
								UINT oldVoxelIndex = ConvertCoordsToId( { x, y, z }, oldObjectDiameter );
								UINT voxelIndex = ConvertCoordsToId( { x, y, z }, objectDiameter );

								voxels[voxelIndex] = oldVoxels[oldVoxelIndex];
							}
						}
					}

					voxelCoords.x = (objectDiameter.x - 1);

					UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );
					voxels[newVoxelIndex].element = 1;
				}
				else
				{
					voxels.resize( 1 );
					voxels[0].element = 1;

					objectPosition =
					{
						objectPosition.x + (voxelCoords.x * voxelDimension) - ((objectDiameter.x) * voxelRadius) + voxelRadius * 3,
						objectPosition.y + (voxelCoords.y * voxelDimension) - ((objectDiameter.y) * voxelRadius) + voxelRadius,
						objectPosition.z + (voxelCoords.z * voxelDimension) - ((objectDiameter.z) * voxelRadius) + voxelRadius
					};

					objectColor = { distribution( generator ), distribution( generator ),  distribution( generator ) };

					objectDiameter = { 1, 1, 1 };

					createNewObject = true;
				}
			}
			else
			{
				++voxelCoords.x;
				UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );

				voxels = object.GetVoxels();
				voxels[newVoxelIndex].element = 1;
			}
			break;
		case GGVoxel::GG_VOXEL_FACE_WEST:
			if( voxelCoords.x == 0 )
			{
				if( objectDiameter.x < GGObject::MAX_DIAMETER )
				{
					XMUINT3 oldObjectDiameter = objectDiameter;
					++objectDiameter.x;
					objectPosition.x -= voxelRadius;

					voxels.resize( objectDiameter.x * objectDiameter.y * objectDiameter.z );

					auto& oldVoxels = object.GetVoxels();
					for( UINT x = 1; x < objectDiameter.x; ++x )
					{
						for( UINT y = 0; y < objectDiameter.y; ++y )
						{
							for( UINT z = 0; z < objectDiameter.z; ++z )
							{
								UINT oldVoxelIndex = ConvertCoordsToId( { x - 1, y, z }, oldObjectDiameter );
								UINT voxelIndex = ConvertCoordsToId( { x, y, z }, objectDiameter );

								voxels[voxelIndex] = oldVoxels[oldVoxelIndex];
							}
						}
					}

					voxelCoords.x = 0;

					UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );
					voxels[newVoxelIndex].element = 1;
				}
				else
				{
					voxels.resize( 1 );
					voxels[0].element = 1;

					objectPosition =
					{
						objectPosition.x + (voxelCoords.x * voxelDimension) - ((objectDiameter.x) * voxelRadius) - voxelRadius,
						objectPosition.y + (voxelCoords.y * voxelDimension) - ((objectDiameter.y) * voxelRadius) + voxelRadius,
						objectPosition.z + (voxelCoords.z * voxelDimension) - ((objectDiameter.z) * voxelRadius) + voxelRadius
					};

					objectColor = { distribution( generator ), distribution( generator ),  distribution( generator ) };

					objectDiameter = { 1, 1, 1 };

					createNewObject = true;
				}
			}
			else
			{
				--voxelCoords.x;
				UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );

				voxels = object.GetVoxels();
				voxels[newVoxelIndex].element = 1;
			}
			break;
		case GGVoxel::GG_VOXEL_FACE_TOP:
			if( voxelCoords.y == (objectDiameter.y - 1) )
			{
				if( objectDiameter.y < GGObject::MAX_DIAMETER )
				{
					XMUINT3 oldObjectDiameter = objectDiameter;
					++objectDiameter.y;

					voxels.resize( objectDiameter.x * objectDiameter.y * objectDiameter.z );

					auto& oldVoxels = object.GetVoxels();
					for( UINT x = 0; x < objectDiameter.x; ++x )
					{
						for( UINT y = 0; y < (objectDiameter.y - 1); ++y )
						{
							for( UINT z = 0; z < objectDiameter.z; ++z )
							{
								UINT oldVoxelIndex = ConvertCoordsToId( { x, y, z }, oldObjectDiameter );
								UINT voxelIndex = ConvertCoordsToId( { x, y, z }, objectDiameter );

								voxels[voxelIndex] = oldVoxels[oldVoxelIndex];
							}
						}
					}

					objectPosition.y += voxelRadius;
					voxelCoords.y = (objectDiameter.y - 1);

					UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );
					voxels[newVoxelIndex].element = 1;
				}
				else
				{
					voxels.resize( 1 );
					voxels[0].element = 1;

					objectPosition =
					{
						objectPosition.x + (voxelCoords.x * voxelDimension) - ((objectDiameter.x) * voxelRadius) + voxelRadius,
						objectPosition.y + (voxelCoords.y * voxelDimension) - ((objectDiameter.y) * voxelRadius) + voxelRadius * 3.0f,
						objectPosition.z + (voxelCoords.z * voxelDimension) - ((objectDiameter.z) * voxelRadius) + voxelRadius
					};

					objectColor = { distribution( generator ), distribution( generator ),  distribution( generator ) };

					objectDiameter = { 1, 1, 1 };

					createNewObject = true;
				}
			}
			else
			{
				++voxelCoords.y;
				UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );

				voxels = object.GetVoxels();
				voxels[newVoxelIndex].element = 1;
			}
			break;
		case GGVoxel::GG_VOXEL_FACE_BOTTOM:
			if( voxelCoords.y == 0 )
			{
				if( objectDiameter.y < GGObject::MAX_DIAMETER )
				{
					XMUINT3 oldObjectDiameter = objectDiameter;
					++objectDiameter.y;
					objectPosition.y -= voxelRadius;

					voxels.resize( objectDiameter.x * objectDiameter.y * objectDiameter.z );

					auto& oldVoxels = object.GetVoxels();
					for( UINT x = 0; x < objectDiameter.x; ++x )
					{
						for( UINT y = 1; y < objectDiameter.y; ++y )
						{
							for( UINT z = 0; z < objectDiameter.z; ++z )
							{
								UINT oldVoxelIndex = ConvertCoordsToId( { x, y - 1, z }, oldObjectDiameter );
								UINT voxelIndex = ConvertCoordsToId( { x, y, z }, objectDiameter );

								voxels[voxelIndex] = oldVoxels[oldVoxelIndex];
							}
						}
					}

					voxelCoords.y = 0;

					UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );
					voxels[newVoxelIndex].element = 1;
				}
				else
				{
					voxels.resize( 1 );
					voxels[0].element = 1;

					objectPosition =
					{
						objectPosition.x + (voxelCoords.x * voxelDimension) - ((objectDiameter.x) * voxelRadius) + voxelRadius,
						objectPosition.y + (voxelCoords.y * voxelDimension) - ((objectDiameter.y) * voxelRadius) - voxelRadius,
						objectPosition.z + (voxelCoords.z * voxelDimension) - ((objectDiameter.z) * voxelRadius) + voxelRadius
					};

					objectColor = { distribution( generator ), distribution( generator ),  distribution( generator ) };

					objectDiameter = { 1, 1, 1 };

					createNewObject = true;
				}
			}
			else
			{
				--voxelCoords.y;
				UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );

				voxels = object.GetVoxels();
				voxels[newVoxelIndex].element = 1;
			}
			break;
		case GGVoxel::GG_VOXEL_FACE_NORTH:
			if( voxelCoords.z == (objectDiameter.z - 1) )
			{
				if( objectDiameter.z < GGObject::MAX_DIAMETER )
				{
					XMUINT3 oldObjectDiameter = objectDiameter;
					++objectDiameter.z;

					voxels.resize( objectDiameter.x * objectDiameter.y * objectDiameter.z );

					auto& oldVoxels = object.GetVoxels();
					for( UINT x = 0; x < objectDiameter.x; ++x )
					{
						for( UINT y = 0; y < objectDiameter.y; ++y )
						{
							for( UINT z = 0; z < (objectDiameter.z - 1); ++z )
							{
								UINT oldVoxelIndex = ConvertCoordsToId( { x, y, z }, oldObjectDiameter );
								UINT voxelIndex = ConvertCoordsToId( { x, y, z }, objectDiameter );

								voxels[voxelIndex] = oldVoxels[oldVoxelIndex];
							}
						}
					}

					objectPosition.z += voxelRadius;
					voxelCoords.z = (objectDiameter.z - 1);

					UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );
					voxels[newVoxelIndex].element = 1;
				}
				else
				{
					voxels.resize( 1 );
					voxels[0].element = 1;

					objectPosition =
					{
						objectPosition.x + (voxelCoords.x * voxelDimension) - ((objectDiameter.x) * voxelRadius) + voxelRadius,
						objectPosition.y + (voxelCoords.y * voxelDimension) - ((objectDiameter.y) * voxelRadius) + voxelRadius,
						objectPosition.z + (voxelCoords.z * voxelDimension) - ((objectDiameter.z) * voxelRadius) + voxelRadius * 3.0f
					};

					objectColor = { distribution( generator ), distribution( generator ),  distribution( generator ) };

					objectDiameter = { 1, 1, 1 };

					createNewObject = true;
				}
			}
			else
			{
				++voxelCoords.z;
				UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );

				voxels = object.GetVoxels();
				voxels[newVoxelIndex].element = 1;
			}
			break;
		case GGVoxel::GG_VOXEL_FACE_SOUTH:
			if( voxelCoords.z == 0 )
			{
				if( objectDiameter.z < GGObject::MAX_DIAMETER )
				{
					XMUINT3 oldObjectDiameter = objectDiameter;
					++objectDiameter.z;
					objectPosition.z -= voxelRadius;

					voxels.resize( objectDiameter.x * objectDiameter.y * objectDiameter.z );

					auto& oldVoxels = object.GetVoxels();
					for( UINT x = 0; x < objectDiameter.x; ++x )
					{
						for( UINT y = 0; y < objectDiameter.y; ++y )
						{
							for( UINT z = 1; z < objectDiameter.z; ++z )
							{
								UINT oldVoxelIndex = ConvertCoordsToId( { x, y, z - 1 }, oldObjectDiameter );
								UINT voxelIndex = ConvertCoordsToId( { x, y, z }, objectDiameter );

								voxels[voxelIndex] = oldVoxels[oldVoxelIndex];
							}
						}
					}

					voxelCoords.z = 0;

					UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );
					voxels[newVoxelIndex].element = 1;
				}
				else
				{
					voxels.resize( 1 );
					voxels[0].element = 1;

					objectPosition =
					{
						objectPosition.x + (voxelCoords.x * voxelDimension) - ((objectDiameter.x) * voxelRadius) + voxelRadius,
						objectPosition.y + (voxelCoords.y * voxelDimension) - ((objectDiameter.y) * voxelRadius) + voxelRadius,
						objectPosition.z + (voxelCoords.z * voxelDimension) - ((objectDiameter.z) * voxelRadius) - voxelRadius
					};

					objectColor = { distribution( generator ), distribution( generator ),  distribution( generator ) };

					objectDiameter = { 1, 1, 1 };

					createNewObject = true;
				}
			}
			else
			{
				--voxelCoords.z;
				UINT newVoxelIndex = ConvertCoordsToId( voxelCoords, objectDiameter );

				voxels = object.GetVoxels();
				voxels[newVoxelIndex].element = 1;
			}
			break;
	}

	if( objectPosition.x >= (GGChunk::DIMENSION / 2.0f) )
	{
		if( _voxelFace.voxel.object.chunk.chunkX == (DIAMETER - 1) )
		{
			return;
		}
		else
		{
			++_voxelFace.voxel.object.chunk.chunkX;
			objectPosition.x -= GGChunk::DIMENSION;
			if( !createNewObject )
			{
				createNewObject = true;
				chunk.RemoveObject( _voxelFace.voxel.object.objectID ); // Remove the old object
			}
		}
	}
	else if( objectPosition.x < (GGChunk::DIMENSION / -2.0f) )
	{
		if( _voxelFace.voxel.object.chunk.chunkX == 0 )
		{
			return;
		}
		else
		{
			--_voxelFace.voxel.object.chunk.chunkX;
			objectPosition.x += GGChunk::DIMENSION;
			if( !createNewObject )
			{
				createNewObject = true;
				chunk.RemoveObject( _voxelFace.voxel.object.objectID ); // Remove the old object
			}
		}
	}

	if( objectPosition.z >= (GGChunk::DIMENSION / 2.0f) )
	{
		if( _voxelFace.voxel.object.chunk.chunkZ == (DIAMETER - 1) )
		{
			return;
		}
		else
		{
			++_voxelFace.voxel.object.chunk.chunkZ;
			objectPosition.z -= GGChunk::DIMENSION;
			if( !createNewObject )
			{
				createNewObject = true;
				chunk.RemoveObject( _voxelFace.voxel.object.objectID ); // Remove the old object
			}
		}
	}
	else if( objectPosition.z < (GGChunk::DIMENSION / -2.0f) )
	{
		if( _voxelFace.voxel.object.chunk.chunkZ == 0 )
		{
			return;
		}
		else
		{
			--_voxelFace.voxel.object.chunk.chunkZ;
			objectPosition.z += GGChunk::DIMENSION;
			if( !createNewObject )
			{
				createNewObject = true;
				chunk.RemoveObject( _voxelFace.voxel.object.objectID ); // Remove the old object
			}
		}
	}

	GGObject newObject = GGObject( move( voxels ), voxelDimension, objectPosition, objectColor, objectDiameter );

	GGChunk& newChunk = GetChunk( _voxelFace.voxel.object.chunk );

	if( createNewObject )
	{
		newChunk.AddObject( move( newObject ) );
	}
	else
	{
		newChunk.ReplaceObject( _voxelFace.voxel.object.objectID, move( newObject ) );
	}

	return;
}

const UINT GGWorld::ConvertCoordsToId( const XMUINT3& _coords, const XMUINT3& _diameter )
{
	return _coords.x * _diameter.y * _diameter.z + _coords.y * _diameter.z + _coords.z;
}

void GGWorld::GenerateChunk( GGChunk& _chunk )
{
	static random_device generator;
	static uniform_real_distribution<float> distribution( 0.25f, 0.75f );

	float voxelDimension = GGChunk::DIMENSION / static_cast<float>(GGObject::MAX_DIAMETER);

	// For testing only
	//XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 position = { voxelDimension / 2.0f, 0.0f, 0.0f };

	XMFLOAT3 color = { distribution( generator ), distribution( generator ),  distribution( generator ) };

	// For testing only
	//_chunk.AddObject( { CreateRandomVoxels(), voxelDimension, position, color } );
	_chunk.AddObject( { CreateRandomVoxels(), voxelDimension, position, color, { GGObject::MAX_DIAMETER - 1, GGObject::MAX_DIAMETER, GGObject::MAX_DIAMETER } } );

	/*static bernoulli_distribution solid( 0.1 );
	if( solid( generator ) )
	{
		voxelDimension /= 4.0f;
		position = { 0.0f, GGChunk::DIMENSION / 2.0f, 0.0f };
		color = { distribution( generator ), distribution( generator ),  distribution( generator ) };
		GGObject smallObject( move( CreateRandomVoxels() ), voxelDimension, position, color );

		_chunk.AddObject( move( smallObject ) );
	}*/

	_chunk.SetState( GGChunk::GG_CHUNK_STATE_GENERAETD );

	return;
}

GGObject::GGVoxels GGWorld::CreateRandomVoxels()
{
	static default_random_engine gen;
	//static bernoulli_distribution solid( 0.025 );
	static bernoulli_distribution solid( 1.0 );

	GGObject::GGVoxels voxels( (GGObject::MAX_DIAMETER - 1) * GGObject::MAX_DIAMETER * GGObject::MAX_DIAMETER );
	for( auto& voxel : voxels )
	{
		if( solid( gen ) )
		{
			voxel.element = 1;
		}
	}

	return voxels;
}

vector<pair<GGWorld::GGChunkDescription, XMFLOAT3>> GGWorld::ChunksToCheckRay( UINT _originChunkX, UINT _originChunkZ, const XMFLOAT3& _originPosition )
{
	vector<pair<GGChunkDescription, XMFLOAT3 >> chunksToCheck;
	chunksToCheck.push_back( { { _originChunkX, _originChunkZ }, _originPosition } );

	if( _originChunkX > 0 )
	{
		UINT chunkX = _originChunkX - 1;
		UINT chunkZ = _originChunkZ;
		XMFLOAT3 pos = _originPosition;
		pos.x += GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}
	if( _originChunkZ > 0 )
	{
		UINT chunkX = _originChunkX;
		UINT chunkZ = _originChunkZ - 1;
		XMFLOAT3 pos = _originPosition;
		pos.z += GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}
	if( _originChunkX > 0 && _originChunkZ > 0 )
	{
		UINT chunkX = _originChunkX - 1;
		UINT chunkZ = _originChunkZ - 1;
		XMFLOAT3 pos = _originPosition;
		pos.x += GGChunk::DIMENSION;
		pos.z += GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}
	if( _originChunkX > 0 && _originChunkZ < DIAMETER - 1 )
	{
		UINT chunkX = _originChunkX - 1;
		UINT chunkZ = _originChunkZ + 1;
		XMFLOAT3 pos = _originPosition;
		pos.x += GGChunk::DIMENSION;
		pos.z -= GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}

	if( _originChunkX < DIAMETER - 1 )
	{
		UINT chunkX = _originChunkX + 1;
		UINT chunkZ = _originChunkZ;
		XMFLOAT3 pos = _originPosition;
		pos.x -= GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}
	if( _originChunkZ < DIAMETER - 1 )
	{
		UINT chunkX = _originChunkX;
		UINT chunkZ = _originChunkZ + 1;
		XMFLOAT3 pos = _originPosition;
		pos.z -= GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}
	if( _originChunkX < DIAMETER - 1 && _originChunkZ < DIAMETER - 1 )
	{
		UINT chunkX = _originChunkX + 1;
		UINT chunkZ = _originChunkZ + 1;
		XMFLOAT3 pos = _originPosition;
		pos.x -= GGChunk::DIMENSION;
		pos.z -= GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}
	if( _originChunkX < DIAMETER - 1 && _originChunkZ > 0 )
	{
		UINT chunkX = _originChunkX + 1;
		UINT chunkZ = _originChunkZ - 1;
		XMFLOAT3 pos = _originPosition;
		pos.x -= GGChunk::DIMENSION;
		pos.z += GGChunk::DIMENSION;

		chunksToCheck.push_back( { { chunkX, chunkZ }, pos } );
	}

	return chunksToCheck;
}

pair<unique_ptr<GGWorld::GGVoxelFaceDescription>, float> GGWorld::GetVoxelFromRayInObject( const XMFLOAT3& _originPos, const XMFLOAT3& _ray, float _maxLength, const GGObject& _object )
{
	const float voxelDimension = _object.GetVoxelDimension();
	const XMUINT3 objectDiameter = _object.GetDiameter();
	const XMFLOAT3 objectRadius = {
		objectDiameter.x * voxelDimension / 2.0f,
		objectDiameter.y * voxelDimension / 2.0f,
		objectDiameter.z * voxelDimension / 2.0f };

	// Ray origin position relative to front-bottom-left corner
	XMFLOAT3 originAbs = {
		(objectRadius.x + _originPos.x - _object.GetPosition().x) / voxelDimension,
		(objectRadius.y + _originPos.y - _object.GetPosition().y) / voxelDimension,
		(objectRadius.z + _originPos.z - _object.GetPosition().z) / voxelDimension };

	// Voxel with ray origin
	XMFLOAT3 voxel = {
		floor( originAbs.x ),
		floor( originAbs.y ),
		floor( originAbs.z ) };

	// Origin position inside the voxel
	XMFLOAT3 originPos = {
		originAbs.x - voxel.x,
		originAbs.y - voxel.y,
		originAbs.z - voxel.z };

	// Ray length to hit the wall
	XMFLOAT3 rayLength;

	// Scale max length to the voxel dimension
	_maxLength /= voxelDimension;

	float totalRayLength = 0.0f;
	while( true )
	{
		if( _ray.x > 0.0f )
		{
			rayLength.x = (1.0f - originPos.x) / _ray.x;
		}
		else if( _ray.x < 0.0f )
		{
			rayLength.x = -originPos.x / _ray.x;
		}
		else
		{
			rayLength.x = INFINITY;
		}

		if( _ray.y > 0.0f )
		{
			rayLength.y = (1.0f - originPos.y) / _ray.y;
		}
		else if( _ray.y < 0.0f )
		{
			rayLength.y = -originPos.y / _ray.y;
		}
		else
		{
			rayLength.y = INFINITY;
		}

		if( _ray.z > 0.0f )
		{
			rayLength.z = (1.0f - originPos.z) / _ray.z;
		}
		else if( _ray.z < 0.0f )
		{
			rayLength.z = -originPos.z / _ray.z;
		}
		else
		{
			rayLength.z = INFINITY;
		}

		GGVoxel::GG_VOXEL_FACE face = GGVoxel::GG_VOXEL_FACE_TOP;

		// Choose the shortest of lengths
		if( rayLength.x <= rayLength.y )
		{
			if( rayLength.x <= rayLength.z )
			{
				if( !signbit( _ray.x ) )
				{
					face = GGVoxel::GG_VOXEL_FACE_WEST;
					++voxel.x;
					originPos.x = 0.0f;
				}
				else
				{
					face = GGVoxel::GG_VOXEL_FACE_EAST;
					--voxel.x;
					originPos.x = 1.0f;
				}

				originPos.y += _ray.y * rayLength.x;
				originPos.z += _ray.z * rayLength.x;
				totalRayLength += rayLength.x;
			}
		}

		if( rayLength.y < rayLength.x )
		{
			if( rayLength.y <= rayLength.z )
			{
				if( !signbit( _ray.y ) )
				{
					face = GGVoxel::GG_VOXEL_FACE_BOTTOM;
					++voxel.y;
					originPos.y = 0.0f;
				}
				else
				{
					face = GGVoxel::GG_VOXEL_FACE_TOP;
					--voxel.y;
					originPos.y = 1.0f;
				}

				originPos.x += _ray.x * rayLength.y;
				originPos.z += _ray.z * rayLength.y;
				totalRayLength += rayLength.y;
			}
		}

		if( rayLength.z < rayLength.x )
		{
			if( rayLength.z < rayLength.y )
			{
				if( !signbit( _ray.z ) )
				{
					face = GGVoxel::GG_VOXEL_FACE_SOUTH;
					++voxel.z;
					originPos.z = 0.0f;
				}
				else
				{
					face = GGVoxel::GG_VOXEL_FACE_NORTH;
					--voxel.z;
					originPos.z = 1.0f;
				}

				originPos.x += _ray.x * rayLength.z;
				originPos.y += _ray.y * rayLength.z;
				totalRayLength += rayLength.z;
			}
		}

		if( totalRayLength > _maxLength )
		{
			break;
		}

		if( (voxel.x >= static_cast<float>(objectDiameter.x)) || (voxel.x < 0.0f) )
		{
			continue;
		}
		if( (voxel.y >= static_cast<float>(objectDiameter.y)) || (voxel.y < 0.0f) )
		{
			continue;
		}
		if( (voxel.z >= static_cast<float>(objectDiameter.z)) || (voxel.z < 0.0f) )
		{
			continue;
		}

		UINT uX = static_cast<UINT>(voxel.x);
		UINT uY = static_cast<UINT>(voxel.y);
		UINT uZ = static_cast<UINT>(voxel.z);
		UINT voxelIndex = uX * objectDiameter.y * objectDiameter.z + uY * objectDiameter.z + uZ;
		assert( voxelIndex < GGObject::MAX_SIZE );
		auto& voxels = _object.GetVoxels();
		if( voxels.at( voxelIndex ).element != 0 )
		{
			GGVoxelFaceDescription faceDesc;
			faceDesc.face = face;
			faceDesc.voxel.voxelX = uX;
			faceDesc.voxel.voxelY = uY;
			faceDesc.voxel.voxelZ = uZ;

			return make_pair( make_unique<GGVoxelFaceDescription>( faceDesc ), totalRayLength * voxelDimension );
		}
	}

	return make_pair( nullptr, INFINITY );
}

bool operator ==( const GGWorld::GGChunkDescription& a, const GGWorld::GGChunkDescription& b )
{
	if( a.chunkX == b.chunkX )
	{
		if( a.chunkZ == b.chunkZ )
		{
			return true;
		}
	}

	return false;
}

bool operator ==( const GGWorld::GGObjectDescription& a, const GGWorld::GGObjectDescription& b )
{
	if( a.chunk == b.chunk )
	{
		if( a.objectID == b.objectID )
		{
			return true;
		}
	}

	return false;
}