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

std::unique_ptr<GGWorld::GGVoxelObjectChunk> GGWorld::GetVoxelFromRay( const DirectX::XMFLOAT3& _originPositin, UINT _originChunkX, UINT _originChunkZ, const DirectX::XMFLOAT3& _rotation, float _length, GGChunk::GGObjectID* _excludedObject )
{
	float voxelDiameter = 1.0f;
	float voxelRadius = voxelDiameter / 2.0f;

	XMVECTOR ray = XMVector3Rotate( XMVectorSet( 0.0f, 0.0f, 1.0f, 1.0f ), XMQuaternionRotationRollPitchYaw( _rotation.x, _rotation.y, _rotation.z ) );
	float rayX = XMVectorGetX( ray );
	float rayY = XMVectorGetY( ray );
	float rayZ = XMVectorGetZ( ray );

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
		float objectRadius = GGObject::DIAMETER / 2.0f;
		XMVECTOR relativePos = XMLoadFloat3( &_originPositin ) - XMLoadFloat3( &object.GetPosition() ); // Relative player position to the center of object

		XMVECTOR objectBounds = XMVectorReplicate( objectRadius );
		if( XMVector3InBounds( relativePos, objectBounds ) )
		{
			// First voxel
			UINT x = static_cast<UINT>(XMVectorGetX( relativePos ) + objectRadius);
			UINT y = static_cast<UINT>(XMVectorGetY( relativePos ) + objectRadius);
			UINT z = static_cast<UINT>(XMVectorGetZ( relativePos ) + objectRadius);

			// Ray origin inside voxel <-0.5 ~ 0.5>
			float voxelX = sgn( XMVectorGetX( relativePos ) ) * (abs( fmod( XMVectorGetX( relativePos ), voxelDiameter ) ) - voxelRadius);
			float voxelY = sgn( XMVectorGetY( relativePos ) ) * (abs( fmod( XMVectorGetY( relativePos ), voxelDiameter ) ) - voxelRadius);
			float voxelZ = sgn( XMVectorGetZ( relativePos ) ) * (abs( fmod( XMVectorGetZ( relativePos ), voxelDiameter ) ) - voxelRadius);

			float totalRayLength = 0.0f;
			while( true )
			{
				// Length of the ray to hit voxel boundary
				float lengthX;
				if( rayX > 0 )
				{
					lengthX = (voxelRadius - voxelX) / rayX;
				}
				else if( rayX < 0 )
				{
					lengthX = -(voxelRadius + voxelX) / rayX;
				}
				else
				{
					lengthX = INFINITY;
				}
				float lengthY;
				if( rayY > 0 )
				{
					lengthY = (voxelRadius - voxelY) / rayY;
				}
				else if( rayY < 0 )
				{
					lengthY = -(voxelRadius + voxelY) / rayY;
				}
				else
				{
					lengthY = INFINITY;
				}
				float lengthZ;
				if( rayZ > 0 )
				{
					lengthZ = (voxelRadius - voxelZ) / rayZ;
				}
				else if( rayZ < 0 )
				{
					lengthZ = -(voxelRadius + voxelZ) / rayZ;
				}
				else
				{
					lengthZ = INFINITY;
				}

				if( lengthX < lengthY )
				{
					if( lengthX < lengthZ )
					{
						if( !signbit( rayX ) )
						{
							++x;
							voxelX = -voxelRadius;
						}
						else
						{
							if( x > 0 )
							{
								--x;
								voxelX = voxelRadius;
							}
							else
							{
								break;
							}
						}

						voxelY += rayY * lengthX;
						voxelZ += rayZ * lengthX;
						totalRayLength += lengthX;
					}
				}

				if( lengthY < lengthX )
				{
					if( lengthY < lengthZ )
					{
						if( !signbit( rayY ) )
						{
							++y;
							voxelY = -voxelRadius;
						}
						else
						{
							if( y > 0 )
							{
								--y;
								voxelY = voxelRadius;
							}
							else
							{
								break;
							}
						}

						voxelX += rayX * lengthY;
						voxelZ += rayZ * lengthY;
						totalRayLength += lengthY;
					}
				}

				if( lengthZ < lengthX )
				{
					if( lengthZ < lengthY )
					{
						if( !signbit( rayZ ) )
						{
							++z;
							voxelZ = -voxelRadius;
						}
						else
						{
							if( z > 0 )
							{
								--z;
								voxelZ = voxelRadius;
							}
							else
							{
								break;
							}
						}

						voxelX += rayX * lengthZ;
						voxelY += +rayY * lengthZ;
						totalRayLength += lengthZ;
					}
				}

				if( x >= GGObject::DIAMETER )
				{
					break;
				}
				if( y >= GGObject::DIAMETER )
				{
					break;
				}
				if( z >= GGObject::DIAMETER )
				{
					break;
				}
				if( totalRayLength > _length )
				{
					break;
				}
				if( totalRayLength == 0.0f )
				{
					break;
				}

				UINT voxelIndex = x * GGObject::DIAMETER * GGObject::DIAMETER + y * GGObject::DIAMETER + z;
				auto& voxels = object.GetVoxels();
				if( voxels.at( voxelIndex ).element != 0 )
				{
					return std::unique_ptr<GGVoxelObjectChunk>(new GGVoxelObjectChunk{ _originChunkX, _originChunkZ, objectID, voxelIndex });
					//return std::make_unique<GGVoxelObjectChunk>( _originChunkX, _originChunkZ, objectID, voxelIndex );
				}
			}
		}
	}

	return nullptr;
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