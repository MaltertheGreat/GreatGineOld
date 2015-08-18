#pragma once

#include <array>
#include <DirectXMath.h>

#include "GGVoxel.h"

class GGObject
{
public:
	static constexpr UINT DIAMETER = 16;
	typedef std::array<GGVoxel, DIAMETER * DIAMETER * DIAMETER> GGVoxelArray;

public:
	GGObject( GGVoxelArray&& _voxels, float _voxelDimension, const DirectX::XMFLOAT3& _position );

public:
	void SetPosition( const DirectX::XMFLOAT3& _pos );
	void SetColor( const DirectX::XMFLOAT3& _color );

	const float GetVoxelDimension() const;
	const DirectX::XMFLOAT3& GetPosition() const;
	const DirectX::XMFLOAT3& GetColor() const;
	const GGVoxelArray& GetVoxels() const;

private:
	float             m_voxelDimension;
	DirectX::XMFLOAT3 m_position;	// Relative to center of chunk
	DirectX::XMFLOAT3 m_color;
	GGVoxelArray      m_voxels;
};