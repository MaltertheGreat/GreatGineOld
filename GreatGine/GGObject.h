#pragma once

#include <vector>
#include <DirectXMath.h>

#include "GGVoxel.h"

class GGObject
{
public:
	static constexpr UINT MAX_DIAMETER = 16;
	static constexpr UINT MAX_SIZE = MAX_DIAMETER * MAX_DIAMETER * MAX_DIAMETER;
	typedef std::vector<GGVoxel> GGVoxels;

public:
	GGObject();
	GGObject( GGVoxels&& _voxels, float _voxelDimension, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _color );

public:
	void SetPosition( const DirectX::XMFLOAT3& _pos );
	void SetColor( const DirectX::XMFLOAT3& _color );

	const bool IsEmpty() const;
	const float GetVoxelDimension() const;
	const DirectX::XMFLOAT3& GetPosition() const;
	const DirectX::XMFLOAT3& GetColor() const;
	const GGVoxels& GetVoxels() const;

private:
	float             m_voxelDimension;
	DirectX::XMFLOAT3 m_position; // Relative to center of chunk
	DirectX::XMFLOAT3 m_color;
	GGVoxels          m_voxels;
};