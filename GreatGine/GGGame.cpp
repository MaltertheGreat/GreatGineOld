#include "PCH.h"
#include "GGGame.h"

void GGGame::Update( GGWorld& _world, float _timeDelta )
{
	switch( m_gameState )
	{
		case GGGame::GG_GAME_STATE_INITIALIZING:
			m_gameState = GG_GAME_STATE_LOADING;
			break;
		case GGGame::GG_GAME_STATE_LOADING:
			m_player.reset( new GGPlayer( _world.GetChunkArray().at( 4 ) ) );
			m_gameState = GG_GAME_STATE_RUNNING;
			break;
		case GGGame::GG_GAME_STATE_RUNNING:
		{
			m_player->Update( _world.GetChunkArray().at( 4 ), _timeDelta );
			break;
		}
	}

	return;
}