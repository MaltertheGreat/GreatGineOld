#include "PCH.h"
#include "GGGUI.h"

GGGUI::GGGUI( UINT _width, UINT _height )
	:
	m_renderable( true ),
	m_width( _width ),
	m_height( _height )
{}

void GGGUI::SetRenderable( bool _renderable )
{
	m_renderable = _renderable;

	return;
}

void GGGUI::AddRectangle( int, int, int, int, float* )
{}

bool GGGUI::IsRenderable() const
{
	return m_renderable;
}