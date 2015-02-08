#pragma once

#include <memory>

#include "GGInputHandler.h"
#include "GGDirectXDriver.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGCamera.h"
#include "GGShader.h"
#include "GGMesh.h"

class GGWindow;

class GGGraphics : public GGInputHandler
{
public:
	GGGraphics( const GGWindow& _window );

public:
	void Update();
	void Render();

public:
	virtual void HandleActionInput( GG_ACTION_INPUT _input, bool _down ) override;

private:
	GGDirectXDriver m_driver;
	GGDevice m_device;
	GGRenderer m_renderer;
	GGCamera m_camera;
	GGShader m_basicShader;
	GGMesh m_mesh;

	DirectX::XMFLOAT3 m_cameraPos;
	DirectX::XMFLOAT3 m_cameraVelocity;
};