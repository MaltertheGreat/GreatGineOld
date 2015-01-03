#include "GGGraphics.h"

GGGraphics::GGGraphics( const GGWindow& _window )
	:
	m_device( _window ),
	m_renderer( m_device )
{}

void GGGraphics::Update()
{
	return;
}

void GGGraphics::Render()
{
	m_renderer.ClearScene();

	m_renderer.PresentScene();

	return;
}