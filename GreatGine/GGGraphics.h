#pragma once

#include "GGInputHandler.h"
#include "GGDirectXDriver.h"
#include "GGDevice.h"
#include "GGRenderer.h"
#include "GGCamera.h"
#include "GGShader.h"
#include "GGMesh.h"
#include "GGFPSCounter.h"

class GGWindow;
class GGConfig;

class GGGraphics : public GGInputHandler
{
public:
	GGGraphics( const GGWindow& _window, const GGConfig& _config );

public:
	void Update( float _frameTime );
	void Render();

public:
	virtual void HandleActionInput( GG_ACTION_INPUT _input, bool _down ) override;
	virtual void HandleRangeInput( int _x, int _y ) override;

private:
	void SwitchFillType();

private:
	GGRenderer::FILL_TYPE m_currentFillType;

	GGDirectXDriver m_driver;
	GGDevice m_device;
	GGRenderer m_renderer;
	GGCamera m_camera;
	GGShader m_basicShader;
	GGMesh m_mesh;
	GGFPSCounter m_fpsCounter;

	DirectX::XMFLOAT3 m_cameraPos = { 0.0f, 0.0f, -5.0f };
	DirectX::XMFLOAT3 m_cameraVelocity;
	DirectX::XMFLOAT3 m_cameraRot;
};