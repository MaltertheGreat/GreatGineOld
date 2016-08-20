#pragma once

#include <vector>
#include <DirectXMath.h>

#include "GGVoxel.h"

class GGObject
{
private:
	struct UCHAR3
	{
		UCHAR x;
		UCHAR y;
		UCHAR z;
	};

public:
	static constexpr UINT MAX_DIAMETER = 12;
	static constexpr UINT MAX_SIZE = MAX_DIAMETER * MAX_DIAMETER * MAX_DIAMETER;
	typedef std::vector<GGVoxel> GGVoxels;

public:
	GGObject( GGVoxels&& _voxels, float _voxelDimension, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _color, const DirectX::XMUINT3& _diameter = { MAX_DIAMETER, MAX_DIAMETER, MAX_DIAMETER } );

public:
	void SetPosition( const DirectX::XMFLOAT3& _pos );
	void SetColor( const DirectX::XMFLOAT3& _color );

	const DirectX::XMUINT3 GetDiameter() const;
	const float GetVoxelDimension() const;
	const DirectX::XMFLOAT3& GetPosition() const;
	const DirectX::XMFLOAT3& GetColor() const;
	const GGVoxels& GetVoxels() const;

private:
	UCHAR3 m_diameter;
	float m_voxelDimension;
	DirectX::XMFLOAT3 m_position; // Relative to center of chunk
	DirectX::XMFLOAT3 m_color;
	const GGVoxels m_voxels;
};