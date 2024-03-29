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
		chunk.Update();

		if( chunk.GetState() == GGChunk::GG_CHUNK_STATE_UNGENERATED )
		{
			GenerateChunk( chunk );
		}
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

void GGWorld::GenerateChunk( GGChunk& _chunk )
{
	static random_device generator;
	static uniform_real_distribution<float> distribution( 0.25f, 0.75f );

	float voxelDimension = GGChunk::DIMENSION / static_cast<float>(GGObject::MAX_DIAMETER);
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 color = { distribution( generator ), distribution( generator ),  distribution( generator ) };

	_chunk.AddObject( { CreateRandomVoxels(), voxelDimension, position, color } );

	static bernoulli_distribution solid( 0.1 );
	if( solid( generator ) )
	{
		voxelDimension /= 4.0f;
		position = { 0.0f, GGChunk::DIMENSION / 2.0f, 0.0f };
		color = { distribution( generator ), distribution( generator ),  distribution( generator ) };
		GGObject smallObject( move( CreateRandomVoxels() ), voxelDimension, position, color );

		_chunk.AddObject( move( smallObject ) );
	}

	_chunk.SetState( GGChunk::GG_CHUNK_STATE_GENERAETD );

	return;
}

GGObject::GGVoxels GGWorld::CreateRandomVoxels()
{
	static default_random_engine gen;
	//static bernoulli_distribution solid( 0.025 );
	static bernoulli_distribution solid( 1.0 );

	GGObject::GGVoxels voxels( GGObject::MAX_SIZE );
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

pair<unique_ptr<GGWorld::GGVoxelFaceDescription>, float> GGWorld::GetVoxelFromRayInObject( const XMFLOAT3& _originPos, const XMFLOAT3& _ray, float _length, const GGObject& _object )
{
	const float voxelDimension = _object.GetVoxelDimension();
	const float voxelRadius = voxelDimension / 2.0f;

	float fX = _originPos.x - _object.GetPosition().x;
	float fY = _originPos.y - _object.GetPosition().y;
	float fZ = _originPos.z - _object.GetPosition().z;

	// First voxel
	int x = static_cast<int>(floor( fX / voxelDimension ));
	int y = static_cast<int>(floor( fY / voxelDimension ));
	int z = static_cast<int>(floor( fZ / voxelDimension ));

	// Ray origin inside voxel <-0.5 ~ 0.5)
	float voxelX;
	float voxelY;
	float voxelZ;
	if( fX < 0.0f )
	{
		voxelX = fmod( fX, voxelDimension ) + voxelRadius;
		if( voxelX == voxelRadius )
			voxelX = -voxelRadius;
	}
	else
	{
		voxelX = fmod( fX, voxelDimension ) - voxelRadius;
	}
	if( fY < 0.0f )
	{
		voxelY = fmod( fY, voxelDimension ) + voxelRadius;
		if( voxelY == voxelRadius )
			voxelY = -voxelRadius;
	}
	else
	{
		voxelY = fmod( fY, voxelDimension ) - voxelRadius;
	}
	if( fZ < 0.0f )
	{
		voxelZ = fmod( fZ, voxelDimension ) + voxelRadius;
		if( voxelZ == voxelRadius )
			voxelZ = -voxelRadius;
	}
	else
	{
		voxelZ = fmod( fZ, voxelDimension ) - voxelRadius;
	}

	float totalRayLength = 0.0f;
	while( true )
	{
		// Length of the ray to hit voxel boundary
		float lengthX;
		if( _ray.x > 0 )
		{
			lengthX = (voxelRadius - voxelX) / _ray.x;
		}
		else if( _ray.x < 0 )
		{
			lengthX = -(voxelRadius + voxelX) / _ray.x;
		}
		else
		{
			lengthX = INFINITY;
		}

		float lengthY;
		if( _ray.y > 0 )
		{
			lengthY = (voxelRadius - voxelY) / _ray.y;
		}
		else if( _ray.y < 0 )
		{
			lengthY = -(voxelRadius + voxelY) / _ray.y;
		}
		else
		{
			lengthY = INFINITY;
		}

		float lengthZ;
		if( _ray.z > 0 )
		{
			lengthZ = (voxelRadius - voxelZ) / _ray.z;
		}
		else if( _ray.z < 0 )
		{
			lengthZ = -(voxelRadius + voxelZ) / _ray.z;
		}
		else
		{
			lengthZ = INFINITY;
		}

		GGVoxel::GG_VOXEL_FACE face = GGVoxel::GG_VOXEL_FACE_TOP;

		// Choose the shortest of lengths
		if( lengthX <= lengthY )
		{
			if( lengthX <= lengthZ )
			{
				if( !signbit( _ray.x ) )
				{
					++x;
					face = GGVoxel::GG_VOXEL_FACE_WEST;
					voxelX = -voxelRadius;
				}
				else
				{
					--x;
					face = GGVoxel::GG_VOXEL_FACE_EAST;
					voxelX = voxelRadius;
				}

				voxelY += _ray.y * lengthX;
				voxelZ += _ray.z * lengthX;
				totalRayLength += lengthX;
			}
		}

		if( lengthY < lengthX )
		{
			if( lengthY <= lengthZ )
			{
				if( !signbit( _ray.y ) )
				{
					++y;
					face = GGVoxel::GG_VOXEL_FACE_BOTTOM;
					voxelY = -voxelRadius;
				}
				else
				{
					--y;
					face = GGVoxel::GG_VOXEL_FACE_TOP;
					voxelY = voxelRadius;
				}

				voxelX += _ray.x * lengthY;
				voxelZ += _ray.z * lengthY;
				totalRayLength += lengthY;
			}
		}

		if( lengthZ < lengthX )
		{
			if( lengthZ < lengthY )
			{
				if( !signbit( _ray.z ) )
				{
					++z;
					face = GGVoxel::GG_VOXEL_FACE_SOUTH;
					voxelZ = -voxelRadius;
				}
				else
				{
					--z;
					face = GGVoxel::GG_VOXEL_FACE_NORTH;
					voxelZ = voxelRadius;
				}

				voxelX += _ray.x * lengthZ;
				voxelY += _ray.y * lengthZ;
				totalRayLength += lengthZ;
			}
		}

		if( totalRayLength > _length )
		{
			break;
		}

		const UINT objectRadius = GGObject::MAX_DIAMETER / 2;
		if( x >= static_cast<int>(objectRadius) || x < -static_cast<int>(objectRadius) )
		{
			continue;
		}
		if( y >= static_cast<int>(objectRadius) || y < -static_cast<int>(objectRadius) )
		{
			continue;
		}
		if( z >= static_cast<int>(objectRadius) || z < -static_cast<int>(objectRadius) )
		{
			continue;
		}

		UINT uX = static_cast<UINT>(x + objectRadius);
		UINT uY = static_cast<UINT>(y + objectRadius);
		UINT uZ = static_cast<UINT>(z + objectRadius);
		UINT voxelIndex = uX * GGObject::MAX_DIAMETER * GGObject::MAX_DIAMETER + uY * GGObject::MAX_DIAMETER + uZ;
		assert( voxelIndex < GGObject::MAX_SIZE );
		auto& voxels = _object.GetVoxels();
		if( voxels.at( voxelIndex ).element != 0 )
		{
			GGVoxelFaceDescription faceDesc;
			faceDesc.face = face;
			faceDesc.voxel.voxelX = uX;
			faceDesc.voxel.voxelY = uY;
			faceDesc.voxel.voxelZ = uZ;

			return make_pair( make_unique<GGVoxelFaceDescription>( faceDesc ), totalRayLength );
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