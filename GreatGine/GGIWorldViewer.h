#pragma once

#include "PCH.h"

class GGIWorldViewer
{
public:
	virtual const DirectX::XMFLOAT3& GetPosition() const = 0;
	virtual const DirectX::XMFLOAT3& GetRotation() const = 0;
};