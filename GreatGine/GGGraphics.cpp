#include "PCH.h"
#include "GGGraphics.h"
#include "GGWindow.h"
#include "GGConfig.h"
#include "GGWorld.h"
using namespace DirectX;
using namespace std;

GGGraphics::GGGraphics( const GGWindow& _window, const GGConfig& _config )
	:
	m_currentFillType( GGRenderer::FILL_TYPE_SOLID ),
	m_driver( _window, _config.GetInt( "resolutionX" ), _config.GetInt( "resolutionY" ) ),
	m_device( m_driver ),
	m_renderer( m_driver, _config.GetInt( "sync_interval" ) ),
	m_camera( m_device.CreateCamera( XMConvertToRadians( _config.GetFloat( "fov" ) ), _config.GetInt( "resolutionX" ), _config.GetInt( "resolutionY" ) ) ),
	m_basicShader( m_device.CreateShader() ),
	m_mesh( m_device.CreateCubeMesh() )
{}

void GGGraphics::Update( const GGWorld& _world, float _frameTime )
{
	const GGIWorldViewer* worldViewer = _world.GetActiveWorldViewer();
	m_device.UpdateCamera( m_camera, worldViewer->GetPosition(), worldViewer->GetRotation() );

	m_debugInfo.Update( _frameTime, worldViewer->GetPosition() );

	return;
}

void GGGraphics::Render()
{
	m_renderer.ClearScene();

	Render3D();
	Render2D();

	m_renderer.PresentScene();

	return;
}

void GGGraphics::HandleActionInput( GG_ACTION_INPUT _input, bool _down )
{
	if( _down )
	{
		if( _input == GG_ACTION_INPUT_FILL_TYPE )
		{
			SwitchFillType();
		}
	}

	return;
}

void GGGraphics::HandleRangeInput( int, int )
{
	return;
}

void GGGraphics::SwitchFillType()
{
	if( m_currentFillType == GGRenderer::FILL_TYPE_SOLID )
	{
		m_currentFillType = GGRenderer::FILL_TYPE_WIREFRAME;
	}
	else
	{
		m_currentFillType = GGRenderer::FILL_TYPE_SOLID;
	}

	return;
}

void GGGraphics::Render3D()
{
	m_renderer.SetFillType( m_currentFillType );

	m_renderer.SetCamera( m_camera );

	m_renderer.SetShader( m_basicShader );

	m_renderer.SetMesh( m_mesh );
	m_renderer.RenderMesh( m_mesh );

	return;
}

void GGGraphics::Render2D()
{
	m_renderer.RenderIn2D();

	for( int i = 0; i < GGDebugInfo::LINE_COUNT; ++i )
	{
		float textY = i * 19.0f;
		m_renderer.RenderText( m_debugInfo.GetLine( i ), { 0.0f, textY } );
	}

	return;
}