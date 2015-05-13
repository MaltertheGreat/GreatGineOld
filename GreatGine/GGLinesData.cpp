#include "PCH.h"
#include "GGLinesData.h"
using namespace DirectX;

GGLinesData GGCubeLines( float _dimension, const DirectX::XMFLOAT3& _color )
{
	float radius = _dimension / 2.0f;

	GGLinesData output;
	output.vertices.assign( {
		{ XMFLOAT3( -radius, radius, -radius ), _color }, // left top front 0
		{ XMFLOAT3( radius, radius, -radius ), _color }, // right top front 1
		{ XMFLOAT3( radius, radius, radius ), _color }, // right top back 2
		{ XMFLOAT3( -radius, radius, radius ), _color }, // left top back 3
		{ XMFLOAT3( -radius, -radius, -radius ), _color }, // left bottom front 4
		{ XMFLOAT3( radius, -radius, -radius ), _color }, // right bottom front 5
		{ XMFLOAT3( radius, -radius, radius ), _color }, // right bottom back 6
		{ XMFLOAT3( -radius, -radius, radius ), _color } // left bottom back 7
	} );

	output.indices.assign( {
		0, 3,
		3, 2,
		2, 1,
		1, 0,

		4, 7,
		7, 6,
		6, 5,
		5, 4,

		0, 4,
		1, 5,
		2, 6,
		3, 7
	} );

	return output;
}