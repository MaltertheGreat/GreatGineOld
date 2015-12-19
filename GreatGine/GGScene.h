#pragma once

#include <map>
#include <array>

#include "GGObjectModel.h"
#include "GGShader.h"
#include "GGWorld.h"

class GGDevice;
class GGCamera;

class GGScene
{
public:
	static constexpr int RENDER_DISTANCE = 1;
	static constexpr int SCENE_DIAMETER = (2 * RENDER_DISTANCE + 1);
	static constexpr int SCENE_SIZE = SCENE_DIAMETER * SCENE_DIAMETER;

	typedef std::vector<GGObjectModel> GGChunkModel;
	typedef std::array<GGChunkModel, SCENE_SIZE> GGChunkModelArray;

	struct GGCamera
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 rotation;
	};

public:
	GGScene( GGDevice& _device );

public:
	void Update( const GGWorld& _world );

	GGWorld::GGChunkDescription GetCenterChunk() const;
	const GGCamera& GetCamera() const;
	const GGChunkModelArray& GetChunkModels() const;

	void SetCenterChunk( GGWorld::GGChunkDescription _chunkDesc );
	void SetCamera( const GGCamera& _camera );

private:
	void UpdateChunkModel( GGChunkModel& _chunkModel, const GGChunk& _chunk, const DirectX::XMFLOAT3& _position );
	void MoveScene( long long _dX, long long _dZ );

private:
	GGDevice& m_device;

	GGWorld::GGChunkDescription m_centerChunk;
	GGCamera m_camera;
	GGChunkModelArray m_chunkModels;

	/*GGChunkModelSets m_chunkModelSets;
	GGShader         m_basicShader;
	GGMesh           m_debugChunkMesh;
	GGShader         m_debugChunkShader;*/
};