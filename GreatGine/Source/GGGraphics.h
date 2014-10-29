#pragma once

#include "GGRenderer.h"
#include "GGWindow.h"

class GGGraphics
{
public:
	GGGraphics( const GGWindow& _window );

	void Update();
	void Render();

private:
	GGRenderer m_renderer;
};