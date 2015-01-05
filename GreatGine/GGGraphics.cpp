#include "GGGraphics.h"
#include "GGWindow.h"

GGGraphics::GGGraphics( const GGWindow& _window )
	:
	m_driver( _window, 800, 600 ),
	m_device( m_driver ),
	m_renderer( m_driver ),
	m_basicShader( m_device.CreateShader() ),
	m_mesh( m_device.CreateTriangleMesh() )
{

}

void GGGraphics::Update()
{
	return;
}

void GGGraphics::Render()
{
	m_renderer.ClearScene();

	m_renderer.SetShader( m_basicShader.get() );

	m_renderer.SetMesh( m_mesh.get() );
	m_renderer.RenderMesh( m_mesh.get() );

	m_renderer.PresentScene();

	return;
}