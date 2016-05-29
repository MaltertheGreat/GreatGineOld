#pragma once

#include "GGObjectData.h"

struct GGPhysicsObjectData : public GGObjectData
{
	static const GGObjectDataID magicID = 1;
	GGPhysicsObjectData( GGObjectDataID _id, bool _physical = true, DirectX::XMFLOAT3 _velocity = { 0.0f, 0.0f, 0.0f } )
		:
		id( _id ),
		physical( _physical ),
		velocity( _velocity )
	{}

	virtual GGObjectDataID GetID()
	{
		return id;
	}

	GGObjectDataID id;
	bool physical;
	DirectX::XMFLOAT3 velocity;
};