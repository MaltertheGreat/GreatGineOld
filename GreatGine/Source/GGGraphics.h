#pragma once

#include "GGWindow.h"
#include "GGRenderer.h"

class GGGraphics
{
public:
	GGGraphics( const GGWindow& _window );

	void Update();
	void Render();

private:
	GGRenderer m_renderer;
};