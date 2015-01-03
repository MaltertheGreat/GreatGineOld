#pragma once

#include "GGWindow.h"
#include "GGDirectXDevice.h"
#include "GGRenderer.h"

class GGGraphics
{
public:
	GGGraphics( const GGWindow& _window );

	void Update();
	void Render();

private:
	GGDirectXDevice m_device;
	GGRenderer m_renderer;
};