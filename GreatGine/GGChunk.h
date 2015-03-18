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
	void SetContent( std::vector<std::unique_ptr<GGDepthLevel>>&& _depthLevels );
	void SetTransformation( DirectX::XMFLOAT4X4& _transformation );

	const DirectX::XMFLOAT4X4& GetTransformation() const;

private:
	bool m_hasChanged;
	std::vector<std::unique_ptr<GGDepthLevel>> m_depthLevels;

	DirectX::XMFLOAT4X4 m_transformation;
};