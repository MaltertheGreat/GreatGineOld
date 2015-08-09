#pragma once

#include <vector>

struct GGVoxel
{
public:
	enum GG_VOXEL_FACE : BYTE
	{
		GG_VOXEL_FACE_TOP = 0,
		GG_VOXEL_FACE_BOTTOM,
		GG_VOXEL_FACE_NORTH,
		GG_VOXEL_FACE_SOUTH,
		GG_VOXEL_FACE_EAST,
		GG_VOXEL_FACE_WEST
	};

public:
	UINT element = 0;
};