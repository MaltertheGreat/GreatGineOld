#include "PCH.h"
#include "GGGame.h"
#include "GGConfig.h"
#include "GGWorld.h"
#include "GGGraphics.h"

GGGame::GGGame( GGInput& _input, GGConfig& _config )
	:
	m_player( _input, _config )
{}

void GGGame::Update( GGWorld& _world, GGGraphics& _graphics, double _timeDelta )
{
	m_player.Update( _world, _timeDelta );

	auto& scene = _graphics.GetScene();

	scene.SetCenterChunk( m_player.GetChunk() );
	scene.SetCamera( { m_player.GetPosition(), m_player.GetRotation() } );

	return;
}