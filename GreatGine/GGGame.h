#pragma once

#include "GGPlayer.h"

class GGInput;
class GGConfig;
class GGWorld;

class GGGame
{
public:
	GGGame( GGInput& _input, GGConfig& _config );

public:
	void Update( GGWorld& _world, float _timeDelta );

private:
	GGPlayer m_player;
};