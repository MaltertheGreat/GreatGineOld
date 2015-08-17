#include "PCH.h"
#include "GGWorld.h"

#include <random>
using namespace DirectX;
using namespace std;

array<XMFLOAT3, 1> test{ { { 0.0f, 0.0f, 0.0f } } };

GGWorld::GGWorld()
	:
	m_renderable( true ),
	m_chunks( InitializeChunks() )
{}

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

void GGWorld::SetRenderable( bool _renderable )
{
	m_renderable = _renderable;

	return;
}

void GGWorld::SetViewPoint( const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation )
{
	m_viewPointPosition = _position;
	m_viewPointRotation = _rotation;

	return;
}

bool GGWorld::IsRenderable() const
{
	return m_renderable;
}

GGChunk& GGWorld::GetChunk( UINT _x, UINT _z )
{
	UINT index = _x * DIAMETER + _z;

	return m_chunks[ index ];
}

GGWorld::GGChunkArray& GGWorld::GetChunkArray()
{
	return m_chunks;
}

const DirectX::XMFLOAT3 & GGWorld::GetViewPointPosition() const
{
	return m_viewPointPosition;
}

const DirectX::XMFLOAT3 & GGWorld::GetViewPointRotation() const
{
	return m_viewPointRotation;
}

float sgn( float f )
{
	if( f > 0.0f )
	{
		return 1.0f;
	}
	else if( f < 0.0f )
	{
		return -1.0f;
	}
	else
	{
		return 0.0f;
	}
}

unique_ptr<GGWorld::GGVoxelDescription> GGWorld::GetVoxelFromRay( UINT _originChunkX, UINT _originChunkZ, const DirectX::XMFLOAT3& _originPosition, const DirectX::XMFLOAT3& _rotation, float _length, GGChunk::GGObjectID* _excludedObject )
{
	XMFLOAT3 ray;
	XMStoreFloat3( &ray, XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 1.0f ), XMQuaternionRotationRollPitchYaw( _rotation.x, _rotation.y, _rotation.z ) ) );

	float minRayLength = INFINITY;
	unique_ptr<GGVoxelDescription> closestVoxelDesc = unique_ptr<GGVoxelDescription>(nullptr);

	auto& objects = GetChunk( _originChunkX, _originChunkZ ).GetObjects();
	for( auto iterator : objects )
	{
		GGChunk::GGObjectID objectID = iterator.first;
		if( _excludedObject )
		{
			if( objectID == *_excludedObject )
			{
				continue;
			}
		}
		auto& object = iterator.second;


		auto tuple = GetVoxelFromRayInObject( _originPosition, ray, _length, object );

		float rayLength = get<1>( tuple );
		unique_ptr<GGVoxelDescription>& voxelDesc = get<0>( tuple );

		if( rayLength < minRayLength )
		{
			minRayLength = rayLength;
			closestVoxelDesc = move( voxelDesc );

			closestVoxelDesc->chunkX = _originChunkX;
			closestVoxelDesc->chunkZ = _originChunkZ;
			closestVoxelDesc->objectID = objectID;
		}
	}

	return move( closestVoxelDesc );
}

GGWorld::GGChunkArray GGWorld::InitializeChunks()
{
	GGChunkArray chunks;

	const float chunkOffset = DIAMETER / 2.0f * GGChunk::DIMENSION - GGChunk::DIMENSION * 0.5f;
	float x = -chunkOffset;
	float z = -chunkOffset;

	for( auto& chunk : chunks )
	{
		chunk = GGChunk( { x, 0.0f, z } );

		XMFLOAT3 position = { x, 0.0f, z };
		z += GGChunk::DIMENSION;
		if( z > chunkOffset )
		{
			z = -chunkOffset;
			x += GGChunk::DIMENSION;
		}
	}

	return chunks;
}

void GGWorld::GenerateChunk( GGChunk& _chunk )
{
	float voxelDimension = GGChunk::DIMENSION / static_cast<float>(GGObject::DIAMETER);
	XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };
	GGObject object( move( CreateRandomVoxels() ), voxelDimension, position );

	_chunk.AddObject( move( object ) );

	static default_random_engine gen;
	static bernoulli_distribution solid( 0.1 );
	if( solid( gen ) )
	{
		voxelDimension /= 4.0f;
		position = { 0.0f, GGChunk::DIMENSION / 2.0f, 0.0f };
		GGObject smallObject( move( CreateRandomVoxels() ), voxelDimension, position );

		_chunk.AddObject( move( smallObject ) );
	}

	_chunk.SetState( GGChunk::GG_CHUNK_STATE_GENERAETD );

	return;
}

GGObject::GGVoxelArray GGWorld::CreateRandomVoxels()
{
	static default_random_engine gen;
	//static bernoulli_distribution solid( 0.025 );
	static bernoulli_distribution solid( 1.0 );

	GGObject::GGVoxelArray voxels;
	for( auto& voxel : voxels )
	{
		if( solid( gen ) )
		{
			voxel.element = 1;
		}
	}

	return voxels;
}

tuple<unique_ptr<GGWorld::GGVoxelDescription>, float> GGWorld::GetVoxelFromRayInObject( const DirectX::XMFLOAT3& _originPos, const DirectX::XMFLOAT3& _ray, float _length, const GGObject& _object )
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

	// Ray origin inside voxel <-0.5 ~ 0.5>
	float voxelX = sgn( fX ) * (abs( fmod( fX, voxelDimension ) ) - voxelRadius);
	float voxelY = sgn( fY ) * (abs( fmod( fY, voxelDimension ) ) - voxelRadius);
	float voxelZ = sgn( fZ ) * (abs( fmod( fZ, voxelDimension ) ) - voxelRadius);

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

		GGVoxel::GG_VOXEL_FACE face;

		// Choose the shortest of lengths
		if( lengthX < lengthY )
		{
			if( lengthX < lengthZ )
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
			if( lengthY < lengthZ )
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
		//if( totalRayLength == 0.0f )
		{
			//break;
		}
		const UINT objectRadius = GGObject::DIAMETER / 2;
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
		UINT voxelIndex = uX * GGObject::DIAMETER * GGObject::DIAMETER + uY * GGObject::DIAMETER + uZ;
		assert( voxelIndex < GGObject::DIAMETER * GGObject::DIAMETER * GGObject::DIAMETER );
		auto& voxels = _object.GetVoxels();
		if( voxels.at( voxelIndex ).element != 0 )
		{
			return make_tuple( unique_ptr<GGWorld::GGVoxelDescription>(new GGVoxelDescription{ NULL, NULL, NULL, uX, uY, uZ, face }), totalRayLength );
		}
	}

	return make_tuple( nullptr, INFINITY );
}
