#include "PCH.h"
#include "GGGraphics.h"
#include "GGWindow.h"
#include "GGConfig.h"
#include "GGMeshData.h"
#include "GGLinesData.h"
using namespace DirectX;
using namespace std;

GGGraphics::GGGraphics( const GGWindow& _window, GGConfig& _config )
	:
	m_resolutionX( _config.GetUint( "resolutionX", 1280 ) ),
	m_resolutionY( _config.GetUint( "resolutionY", 720 ) ),
	m_renderFlags( 0b110 ),

	m_keyMap{ _config.GetUint( "key_fill_type", VK_F1 ), _config.GetUint( "key_render_chunks", VK_F2 ) },
	m_currentFillType( GGRenderer::FILL_TYPE_SOLID ),

	m_driver( _window, m_resolutionX, m_resolutionY ),
	m_device( m_driver ),
	m_renderer( m_driver, _config.GetInt( "sync_interval", 0 ) ),
	m_camera( m_device.CreateCamera( _config.GetFloat( "fov", 80.0f ), m_resolutionX, m_resolutionY ) ),
	m_basicShader( m_device.CreateShader() ),
	m_debugChunkMesh( m_device.CraeteLinesMesh( VerticalLine() ) ),
	m_debugChunkShader( m_device.CreateLinesShader() )
{}

void GGGraphics::Update( GGWorld& _world, float _frameTime )
{
	m_device.UpdateCamera( m_camera, _world.GetViewPointPosition(), _world.GetViewPointRotation() );

	auto& chunks = _world.GetChunkArray();
	for( UINT i = 0; i < (GGWorld::DIAMETER * GGWorld::DIAMETER); ++i )
	{
		m_chunkModelSets[ i ].Update( m_device, chunks[ i ] );
	}

	m_debugInfo.Update( _frameTime, _world.GetViewPointPosition(), _world.GetViewPointRotation() );

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

void GGGraphics::HandleKeyInput( WPARAM _keyCode, bool _down )
{
	if( _down )
	{
		if( _keyCode == m_keyMap[ GG_KEYMAP_FILL_TYPE ] )
		{
			SwitchFillType();
		}
		else if( _keyCode == m_keyMap[ GG_KEYMAP_RENDER_CHUNKS ] )
		{
			m_renderFlags[GG_RENDER_FLAGS_DEBUG].flip();
		}
	}

	return;
}

void GGGraphics::HandleMouseInput( int, int )
{
	return;
}

GGLinesData GGGraphics::VerticalLine()
{
	GGLinesData output;
	XMFLOAT3 color = { 1.0f, 0.749f, 0.0f };

	output.vertices.assign( {
		{ { 0.0f, -50.0f, 0.0f}, color },
		{ { 0.0f, 50.0f, 0.0f }, color }
	} );

	output.indices.assign( {
		0, 1
	} );

	return output;
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
	m_renderer.SetRenderType( GGRenderer::RENDER_TYPE_MESH );
	m_renderer.SetFillType( m_currentFillType );
	m_renderer.SetCamera( m_camera );

	if( m_renderFlags[ GG_RENDER_FLAGS_WORLD ] )
	{
		m_renderer.SetShader( m_basicShader );
		for( auto& modelSet : m_chunkModelSets )
		{
			modelSet.Render( m_renderer );
		}
	}

	if( m_renderFlags[ GG_RENDER_FLAGS_DEBUG ] )
	{
		m_renderer.SetRenderType( GGRenderer::RENDER_TYPE_LINES );
		m_renderer.SetShader( m_debugChunkShader );

		m_renderer.SetMesh( m_debugChunkMesh );

		const float worldRadius = GGWorld::DIAMETER / 2.0f * GGChunk::DIMENSION;

		float x = -worldRadius;
		while( x <= worldRadius )
		{
			float z = -worldRadius;
			while( z <= worldRadius )
			{
				XMFLOAT4X4 transformation;

				XMMATRIX matrix = XMMatrixTranslation( x, 0.0f, z );
				XMStoreFloat4x4( &transformation, matrix );

				m_renderer.RenderMesh( m_debugChunkMesh, transformation );

				z += GGChunk::DIMENSION;
			}

			x += GGChunk::DIMENSION;
		}
	}

	return;
}

void GGGraphics::Render2D()
{
	m_renderer.RenderIn2D();

	if( m_renderFlags[ GG_RENDER_FLAGS_GUI ] )
	{
		for( int i = 0; i < GGDebugInfo::LINE_COUNT; ++i )
		{
			float textY = i * 19.0f;
			m_renderer.RenderText( m_debugInfo.GetLine( i ), { 0.0f, textY } );
		}
	}

	return;
}