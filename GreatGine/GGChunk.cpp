#include "PCH.h"
#include "GGChunk.h"
using namespace DirectX;
using namespace std;

GGChunk::GGChunk()
	:
	m_hasChanged( true )
{}

void GGChunk::SetChangeState( bool _hasChanged )
{
	m_hasChanged = _hasChanged;

	return;
}

void GGChunk::SetContent( vector<std::unique_ptr<GGDepthLevel>>& _depthLevels )
{
	m_depthLevels = move( _depthLevels );

	return;
}

void GGChunk::SetPosition( const DirectX::XMFLOAT3& _position )
{
	m_position = _position;

	return;
}

bool GGChunk::HasChanged() const
{
	return m_hasChanged;
}

const vector<std::unique_ptr<GGDepthLevel>>& GGChunk::GetDepthLevels() const
{
	return m_depthLevels;
}

const XMFLOAT3& GGChunk::GetPosition() const
{
	return m_position;
}