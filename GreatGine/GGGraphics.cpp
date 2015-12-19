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

	m_device( _window, m_resolutionX, m_resolutionY ),
	m_renderer( m_device.GetDriver(), _config.GetInt( "sync_interval", 0 ) ),
	m_scene( m_device ),
	m_basicShader( m_device.CreateShader() )
	//m_debugChunkMesh( m_device.CraeteLinesMesh( GGLinesData::VerticalLine( 50.0f, { 1.0f, 0.749f, 0.0f } ) ) ),
	//m_debugChunkShader( m_device.CreateLinesShader() )
{
	m_renderer.SetCamera( _config.GetFloat( "fov", 80.0f ), m_resolutionX, m_resolutionY );
}

void GGGraphics::Update( GGWorld& _world, double _timeDelta )
{
	/*auto& chunks = _world.GetChunkArray();
	for( UINT i = 0; i < (GGWorld::DIAMETER * GGWorld::DIAMETER); ++i )
	{
		m_chunkModelSets[ i ].Update( m_device, chunks[ i ] );
	}*/

	m_scene.Update( _world );

	auto& camera = m_scene.GetCamera();

	m_debugInfo.Update( _timeDelta, camera.position, camera.rotation );
	// Camera from scene
	m_renderer.UpdateCamera( camera.position, camera.rotation );

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

UINT GGGraphics::GetResolutionX() const
{
	return m_resolutionX;
}

UINT GGGraphics::GetResolutionY() const
{
	return m_resolutionY;
}

GGScene& GGGraphics::GetScene()
{
	return m_scene;
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
			m_renderFlags[ GG_RENDER_FLAGS_DEBUG ].flip();
		}
	}

	return;
}

void GGGraphics::HandleMouseInput( int, int )
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
	m_renderer.SetRenderType( GGRenderer::RENDER_TYPE_MESH );
	m_renderer.SetFillType( m_currentFillType );

	m_renderer.SetShader( m_basicShader );
	auto& chunkModels = m_scene.GetChunkModels();
	for( auto& chunk : chunkModels )
	{
		for( auto& objectModel : chunk )
		{
			if( !objectModel.IsEmpty() )
			{
				m_renderer.SetMesh( objectModel.GetMesh() );
				m_renderer.RenderMesh( objectModel.GetMesh(), objectModel.GetTransformation() );
			}
		}
	}

	/*if( m_renderFlags[ GG_RENDER_FLAGS_DEBUG ] )
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
	}*/

	return;
}

void GGGraphics::Render2D()
{
	m_renderer.Begin2DRendering();

	if( m_renderFlags[ GG_RENDER_FLAGS_GUI ] )
	{
		for( int i = 0; i < GGDebugInfo::LINE_COUNT; ++i )
		{
			float textY = i * 19.0f;
			m_renderer.RenderText( m_debugInfo.GetLine( i ), { 0.0f, textY } );
		}
	}

	XMFLOAT2 cursorPos = { (m_resolutionX / 2.0f) - 2.0f, (m_resolutionY / 2.0f) - 2.0f };

	m_renderer.RenderEllipse( cursorPos, 1.5f, 1.5f, { 0.5f, 0.5f, 0.5f, 0.5f } );

	m_renderer.End2DRendering();

	return;
}