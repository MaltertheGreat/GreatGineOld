#include "PCH.h"
#include "GGScene.h"
#include "GGDevice.h"

using namespace DirectX;

GGScene::GGScene( GGDevice& _device )
	:
	m_device( _device )
{}

void GGScene::Update( const GGWorld& _world )
{
	long long chunkX = static_cast<long long>(m_centerChunk.chunkX) - RENDER_DISTANCE;
	for( int x = 0; x < SCENE_DIAMETER; ++x )
	{
		long long chunkZ = static_cast<long long>(m_centerChunk.chunkZ) - RENDER_DISTANCE;
		for( int z = 0; z < SCENE_DIAMETER; ++z )
		{
			if( chunkX >= 0 && chunkX < GGWorld::DIAMETER && chunkZ >= 0 && chunkZ < GGWorld::DIAMETER )
			{
				UINT index = x * SCENE_DIAMETER + z;
				auto& chunkModel = m_chunkModels[ index ];

				auto& chunk = _world.GetChunk( { static_cast<UINT>(chunkX), static_cast<UINT>(chunkZ) } );

				XMFLOAT3 position = { (x - RENDER_DISTANCE) * GGChunk::DIMENSION, 0.0f, (z - RENDER_DISTANCE) * GGChunk::DIMENSION };

				UpdateChunkModel( chunkModel, chunk, position );
			}

			++chunkZ;
		}

		++chunkX;
	}
}

GGWorld::GGChunkDescription GGScene::GetCenterChunk() const
{
	return m_centerChunk;
}

const GGScene::GGCamera& GGScene::GetCamera() const
{
	return m_camera;
}

const GGScene::GGChunkModelArray& GGScene::GetChunkModels() const
{
	return m_chunkModels;
}

void GGScene::SetCenterChunk( GGWorld::GGChunkDescription _chunkDesc )
{
	MoveScene( static_cast<long long>(m_centerChunk.chunkX) - static_cast<long long>(_chunkDesc.chunkX), static_cast<long long>(m_centerChunk.chunkZ) - static_cast<long long>(_chunkDesc.chunkZ) );

	m_centerChunk = _chunkDesc;

	return;
}

void GGScene::SetCamera( const GGCamera& _camera )
{
	m_camera = _camera;

	return;
}

void GGScene::UpdateChunkModel( GGChunkModel& _chunkModel, const GGChunk& _chunk, const DirectX::XMFLOAT3& _position )
{
	if( _chunkModel.empty() )
	{
		for( auto& object : _chunk.GetObjects() )
		{
			_chunkModel[ object.first ] = GGObjectModel( m_device, object.second, _position, object.second.GetColor() );
		}
	}
	else
	{
		auto& newObjectIDs = _chunk.GetAddedObjectIDs();
		for( auto& id : newObjectIDs )
		{
			_chunkModel[ id ] = GGObjectModel( m_device, _chunk.GetObjects().at( id ), _position, _chunk.GetObjects().at( id ).GetColor() );
		}

		auto& modifiedObjectIDs = _chunk.GetModifiedObjectIDs();
		for( auto& id : modifiedObjectIDs )
		{
			_chunkModel.at( id ).Update( _chunk.GetObjects().at( id ), _position );
		}

		auto& removedObjectIDs = _chunk.GetRemovedObjectIDs();
		for( auto& id : removedObjectIDs )
		{
			_chunkModel.erase( id );
		}
	}

	return;
}

void GGScene::MoveScene( long long _dX, long long _dZ )
{
	if( _dX || _dZ )
	{
		GGChunkModelArray newChunkModels;

		for( int x = 0; x < SCENE_DIAMETER; ++x )
		{
			int newX = static_cast<int>(x + _dX);
			if( newX >= 0 && newX < SCENE_DIAMETER )
			{
				for( int z = 0; z < SCENE_DIAMETER; ++z )
				{
					int newZ = static_cast<int>(z + _dZ);
					if( newZ >= 0 && newZ < SCENE_DIAMETER )
					{
						auto& newChunkModel = newChunkModels[ newX * SCENE_DIAMETER + newZ ];
						newChunkModel = std::move( m_chunkModels[ x * SCENE_DIAMETER + z ] );
						for( auto& objectModel : newChunkModel )
						{
							objectModel.second.Move( { _dX * GGChunk::DIMENSION, 0.0f, _dZ * GGChunk::DIMENSION } );
						}
					}
				}
			}
		}

		m_chunkModels = move( newChunkModels );
	}

	return;
}