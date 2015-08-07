#include "PCH.h"
#include "GGMeshData.h"
using namespace DirectX;

GGMeshData GGMeshData::GGCubeMeshData( float _dimension )
{
	float radius = _dimension / 2.0f;

	GGMeshData output;
	output.vertices.assign( {
		{ XMFLOAT3( -radius, radius, -radius ) },
		{ XMFLOAT3( radius, radius, -radius ) },
		{ XMFLOAT3( radius, radius, radius ) },
		{ XMFLOAT3( -radius, radius, radius ) },
		{ XMFLOAT3( -radius, -radius, -radius ) },
		{ XMFLOAT3( radius, -radius, -radius ) },
		{ XMFLOAT3( radius, -radius, radius ) },
		{ XMFLOAT3( -radius, -radius, radius ) }
	} );

	output.indices.assign( {
		3, 1, 0,
		2, 1, 3,

		0, 5, 4,
		1, 5, 0,

		3, 4, 7,
		0, 4, 3,

		1, 6, 5,
		2, 6, 1,

		2, 7, 6,
		3, 7, 2,

		6, 4, 5,
		7, 4, 6,
	} );

	return output;
}