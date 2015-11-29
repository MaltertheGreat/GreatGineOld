#pragma once

#include "GGPlayer.h"

class GGInput;
class GGConfig;
class GGWorld;
class GGGraphics;

class GGGame
{
public:
	GGGame( GGInput& _input, GGConfig& _config );

public:
	void Update( GGWorld& _world, GGGraphics& _graphics, double _timeDelta );

private:
	GGPlayer m_player;
};