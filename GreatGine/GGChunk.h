#pragma once

#include <array>
#include <DirectXMath.h>

#include "GGVoxel.h"

class GGChunk
{
public:
	static const UINT DIMENSION = 16;
	typedef std::array<GGVoxel, DIMENSION * DIMENSION * DIMENSION> GGVoxelArray;

public:
	GGChunk();
	GGChunk( GGVoxelArray&& _voxels, const DirectX::XMFLOAT3& _position );

public:
	void SetChangeState( bool _state );

	bool HasChanged() const;
	const DirectX::XMFLOAT3& GetPosition() const;
	const GGVoxelArray& GetVoxels() const;

private:
	bool m_hasChanged;
	DirectX::XMFLOAT3 m_position;
	GGVoxelArray m_voxels;
};