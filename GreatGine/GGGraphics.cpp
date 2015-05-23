#include "PCH.h"
#include "GGGraphics.h"
#include "GGWindow.h"
#include "GGConfig.h"
#include "GGMeshData.h"
#include "GGLinesData.h"
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
	m_debugChunkMesh( m_device.CraeteLinesMesh( VerticalLine() ) ),
	m_debugChunkShader( m_device.CreateLinesShader() )
{}

void GGGraphics::Update( GGWorld& _world, float _frameTime )
{
	const GGIWorldViewer* worldViewer = _world.GetActiveWorldViewer();
	m_device.UpdateCamera( m_camera, worldViewer->GetPosition(), worldViewer->GetRotation() );

	auto& chunks = _world.GetChunkArray();
	for( UINT i = 0; i < (GGWorld::DIAMETER * GGWorld::DIAMETER); ++i )
	{
		m_chunkModelSets[ i ].Update( m_device, chunks[ i ] );
	}

	m_debugInfo.Update( _frameTime, worldViewer->GetPosition(), worldViewer->GetRotation() );

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
		switch( _input )
		{
			case GG_ACTION_INPUT_FILL_TYPE:
				SwitchFillType();
				break;
			case GG_ACTION_INPUT_RENDER_CHUNKS:
				m_renderChunks = !m_renderChunks;
				break;
		}
	}

	return;
}

void GGGraphics::HandleRangeInput( int, int )
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

	m_renderer.SetShader( m_basicShader );
	for( auto& modelSet : m_chunkModelSets )
	{
		modelSet.Render( m_renderer );
	}

	if( m_renderChunks )
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

		/*const float chunkOffset = -(GGWorld::DIAMETER / 2.0f * GGChunk::DIMENSION - GGChunk::DIMENSION * 0.5f);

		for( UINT x = 0; x < GGWorld::DIAMETER; ++x )
		{
			for( UINT z = 0; z < GGWorld::DIAMETER; ++z )
			{
				float posX = chunkOffset + x * GGChunk::DIMENSION;
				float posZ = chunkOffset + z * GGChunk::DIMENSION;

				XMFLOAT4X4 transformation;

				XMMATRIX matrix = XMMatrixTranslation( posX, 0.0f, posZ );
				XMStoreFloat4x4( &transformation, matrix );

				m_renderer.RenderMesh( m_debugChunkMesh, transformation );
			}
		}*/
	}

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