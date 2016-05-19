#pragma once

#include "GGObjectData.h"

struct GGPhysicsObjectData : public GGObjectData
{
	static const GGObjectDataID magicID = 1;
	GGPhysicsObjectData( GGObjectDataID _id, DirectX::XMFLOAT3& _velocity )
		:
		id( _id ),
		velocity( _velocity )
	{}

	virtual GGObjectDataID GetID()
	{
		return id;
	}

	GGObjectDataID id;
	DirectX::XMFLOAT3 velocity;
};