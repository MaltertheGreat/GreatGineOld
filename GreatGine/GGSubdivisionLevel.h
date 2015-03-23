#pragma once

#include <array>
#include <bitset>

#include "GGVoxel.h"

struct GGSubdivisionLevel
{
	std::array<GGVoxel, 8> voxels;
	std::bitset<8> subdivisions;
};