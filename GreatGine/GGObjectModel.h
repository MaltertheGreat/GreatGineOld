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
	void Create( const GGDevice& _device, const GGObject& _object );

	const GGMesh* GetMesh() const;
	const DirectX::XMFLOAT4X4& GetTransformation() const;

private:
	GGMeshData CreateObjectGrid( const GGObject& _object );
	void CreateVoxel( UINT x, UINT y, UINT z, const GGObject::GGVoxelArray& voxels, GGMeshData& _grid );

private:
	std::unique_ptr<GGMesh> m_mesh;
	DirectX::XMFLOAT4X4 m_transformation;
};