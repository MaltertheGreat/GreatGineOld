#pragma once

#include "PCH.h"

struct GGMeshData
{
public:
	struct GGVertex
	{
		DirectX::XMFLOAT3 outputpos;
	};

	typedef unsigned int GGIndex;

public:
	std::vector<GGVertex> vertices;
	std::vector<GGIndex> indices;
};

GGMeshData GGCubeMeshData( float _diameter );