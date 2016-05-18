#pragma once

struct GGObjectData
{
	typedef unsigned GGObjectDataID;
	virtual GGObjectDataID GetID() = 0;
};