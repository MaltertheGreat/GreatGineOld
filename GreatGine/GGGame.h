#pragma once

#include <memory>

#include "GGWorld.h"
#include "GGPlayer.h"

class GGGame
{
public:
	enum GG_GAME_STATE
	{
		GG_GAME_STATE_LOADING,
		GG_GAME_STATE_RUNNING
	};

public:
	void Update( GGWorld& _world, float _timeDelta );

private:
	GG_GAME_STATE m_gameState = GG_GAME_STATE_LOADING;
	std::unique_ptr<GGPlayer> m_player;
};