#pragma once

#include "PCH.h"
#include "GGRenderer.h"
#include "GGMesh.h"
#include "GGObject.h"
#include "GGMeshData.h"

class GGDevice;

class GGObjectModel
{
public:
	GGObjectModel();
	GGObjectModel( const GGDevice& _device, const GGObject& _object, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _color );

	void Update( const GGObject& _object, const DirectX::XMFLOAT3& _position );

	const GGMesh GetMesh() const;
	const DirectX::XMFLOAT4X4& GetTransformation() const;

private:
	GGMeshData CreateObjectGrid( const GGObject& _object, const DirectX::XMFLOAT3& _color );
	void CreateVoxel( UINT x, UINT y, UINT z, const GGObject::GGVoxelArray& voxels, GGMeshData& _grid, const DirectX::XMFLOAT3& _color );

private:
	GGMesh m_mesh;
	DirectX::XMFLOAT4X4 m_transformation;
};