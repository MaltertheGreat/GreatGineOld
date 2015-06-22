#include "PCH.h"
#include "GGGame.h"
#include "GGConfig.h"

GGGame::GGGame( GGInput& _input, GGConfig& _config )
	:
	m_player( _input, _config )
{}

void GGGame::Update( GGWorld& _world, float _timeDelta )
{
	m_player.Update( _world, _timeDelta );
	_world.SetCamera( m_player.GetPosition(), m_player.GetRotation() );

	return;
}