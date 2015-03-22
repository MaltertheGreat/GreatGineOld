#pragma once

#include <vector>
#include <memory>
#include <DirectXMath.h>

#include "GGDepthLevel.h"

class GGChunk
{
public:
	GGChunk();

public:
	void SetChangeState( bool _hasChanged );
	void SetContent( std::vector<std::unique_ptr<GGDepthLevel>>&& _depthLevels );
	void SetPosition( const DirectX::XMFLOAT3& _position );

	bool HasChanged() const;
	const std::vector<std::unique_ptr<GGDepthLevel>>& GetDepthLevels() const;
	const DirectX::XMFLOAT3& GetPosition() const;

private:
	bool m_hasChanged;
	std::vector<std::unique_ptr<GGDepthLevel>> m_depthLevels;

	DirectX::XMFLOAT3 m_position;
};