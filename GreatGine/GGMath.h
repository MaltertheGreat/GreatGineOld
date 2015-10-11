#pragma once

#include <DirectXMath.h>

struct GGIntVector
{
	int x;
	int y;
	int z;
};

constexpr float GGConvertToSI( int _gg );
constexpr int GGConvertToGG( float _si );

DirectX::XMFLOAT3 GGConvertToSI( const DirectX::XMINT3& _vector );
DirectX::XMINT3 GGConvertToGG( const DirectX::XMFLOAT3& _vector );