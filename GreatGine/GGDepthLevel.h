#pragma once

#include <vector>

#include "GGVoxel.h"

struct GGDepthLevel
{
public:
	GGDepthLevel( std::vector<GGVoxel>&& _voxels, std::vector<bool>&& _subdivisions );

	std::vector<GGVoxel> voxels;
	std::vector<bool> subdivisions;
};