#pragma once

#include <vector>
#include <DirectXMath.h>
#include <wrl/client.h>

struct GGLinesData
{
public:
	struct GGVertex
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT3 color;
	};

	typedef unsigned int GGIndex;

public:
	std::vector<GGVertex> vertices;
	std::vector<GGIndex> indices;

public:
	static GGLinesData GGCubeLines( float _dimension, const DirectX::XMFLOAT3& _color );
	static GGLinesData VerticalLine( float _radius, const DirectX::XMFLOAT3& _color );
};