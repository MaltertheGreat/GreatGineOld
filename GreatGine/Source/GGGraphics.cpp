#include "GGGraphics.h"

GGGraphics::GGGraphics( const GGWindow& _window )
	:
	m_renderer( _window, _window.GetWidth(), _window.GetHeight() )
{
}

void GGGraphics::Update()
{
}

void GGGraphics::Render()
{
	float color[ 4 ] = { 0.2f, 0.2f, 0.8f, 1.0f };
	m_renderer.ClearScene( color );

	m_renderer.PresentScene();
}
