#include "PCH.h"
#include "GGGame.h"
#include "GGConfig.h"
#include "GGWorld.h"

GGGame::GGGame( GGInput& _input, GGConfig& _config )
	:
	m_player( _input, _config )
{}

void GGGame::Update( GGWorld& _world, double _timeDelta )
{
	m_player.Update( _world, _timeDelta );

	_world.SetViewPoint( m_player.GetPosition(), m_player.GetRotation() );

	return;
}