#pragma once

#include <vector>

#include "GGObject.h"

class GGChunk
{
public:
	static constexpr float DIMENSION = 16.0f;

public:
	void AddObject( GGObject&& _object );

	const std::vector<GGObject>& GetObjects() const;

private:
	std::vector<GGObject> m_objects;
};