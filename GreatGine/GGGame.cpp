#include "PCH.h"
#include "GGGame.h"

void GGGame::Update( GGWorld& _world, float _timeDelta )
{
	if( m_gameState != GG_GAME_STATE_RUNNING )
	{
		m_player.reset( new GGPlayer( _world.GetChunkArray().at( 4 ) ) );
		m_gameState = GG_GAME_STATE_RUNNING;
	}

	m_player->Update( _world.GetChunkArray().at( 4 ), _timeDelta );

	return;
}