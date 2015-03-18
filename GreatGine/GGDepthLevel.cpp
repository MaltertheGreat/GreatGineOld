#include "PCH.h"
#include "GGDepthLevel.h"
using namespace std;

GGDepthLevel::GGDepthLevel( vector<GGVoxel>&& _voxels, vector<bool>&& _subdivisions )
	:
	voxels( _voxels ),
	subdivisions( _subdivisions )
{

}