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
	GGObjectModel( const GGDevice& _device, const GGObject& _object, const DirectX::XMFLOAT3& _position );

	void Update( const GGObject& _object, const DirectX::XMFLOAT3& _position );
	void Move( const DirectX::XMFLOAT3& _offset );

	bool IsEmpty() const;
	const GGMesh GetMesh() const;
	const DirectX::XMFLOAT4X4& GetTransformation() const;

private:
	GGMeshData CreateObjectGrid( const GGObject& _object, const DirectX::XMFLOAT3& _color );
	void CreateVoxel( UINT x, UINT y, UINT z, const GGObject::GGVoxels& voxels, GGMeshData& _grid, const DirectX::XMFLOAT3& _color );

private:
	GGMesh m_mesh;
	DirectX::XMFLOAT4X4 m_transformation;
};