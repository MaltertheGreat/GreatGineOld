#pragma once

#include <vector>

struct GGVoxel
{
public:
	enum GG_VOXEL_FACE : BYTE
	{
		GG_VOXEL_FACE_EAST = 0,
		GG_VOXEL_FACE_WEST,
		GG_VOXEL_FACE_TOP,
		GG_VOXEL_FACE_BOTTOM,
		GG_VOXEL_FACE_NORTH,
		GG_VOXEL_FACE_SOUTH
	};

public:
	UINT element = 0;
};