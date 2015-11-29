#pragma once

#include <vector>
#include <DirectXMath.h>

struct GGMeshData
{
public:
	struct GGVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 normal;
		DirectX::XMFLOAT3 color;
	};

	typedef unsigned int GGIndex;

public:
	std::vector<GGVertex> vertices;
	std::vector<GGIndex> indices;

public:
	static GGMeshData GGCubeMeshData( float _dimension );
};