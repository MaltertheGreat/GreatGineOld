#include "PCH.h"
#include "GGMath.h"
using namespace DirectX;

constexpr float GGConvertToSI( int _gg )
{
	return _gg / (65536.0f);
}

constexpr int GGConvertToGG( float _si )
{
	return _si * (65536);
}

XMFLOAT3 GGConvertToSI( const XMINT3& _vector )
{
	DirectX::XMFLOAT3 result;
	result.x = GGConvertToSI( _vector.x );
	result.y = GGConvertToSI( _vector.y );
	result.z = GGConvertToSI( _vector.z );

	return result;
}

XMINT3 GGConvertToGG( const XMFLOAT3 & _vector )
{
	XMINT3 result;
	result.x = GGConvertToGG( _vector.x );
	result.y = GGConvertToGG( _vector.y );
	result.z = GGConvertToGG( _vector.z );

	return result;
}
