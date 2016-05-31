#pragma once

#include "GGObjectData.h"

struct GGPhysicsObjectData : public GGObjectData
{
	static const GGObjectDataID ID = 1;

	GGPhysicsObjectData( bool _physical = true, DirectX::XMFLOAT3 _velocity = { 0.0f, 0.0f, 0.0f } )
		:
		physical( _physical ),
		velocity( _velocity )
	{}

	bool physical;
	DirectX::XMFLOAT3 velocity;
};