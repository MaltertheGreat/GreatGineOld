#include "GGGraphics.h"

GGGraphics::GGGraphics( const GGWindow& _window )
	:
	m_renderer( _window )
{
}

void GGGraphics::Update()
{
}

void GGGraphics::Render()
{
	m_renderer.ClearScene();

	m_renderer.PresentScene();

	return;
}
