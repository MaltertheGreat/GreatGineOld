#pragma once

#include <memory>

#include "GGDirectXDriver.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGBasicShader.h"

class GGWindow;

class GGGraphics
{
public:
	GGGraphics( const GGWindow& _window );

public:
	void Update();
	void Render();

private:
	GGDirectXDriver m_driver;
	GGDevice m_device;
	GGRenderer m_renderer;

	std::unique_ptr<GGBasicShader> m_basicShader;
};