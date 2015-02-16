#pragma once

#include "PCH.h"

class GGCamera
{
public:
	GGCamera( Microsoft::WRL::ComPtr<ID3D11Buffer> _viewBuffer, Microsoft::WRL::ComPtr<ID3D11Buffer> _projectionBuffer );

public:

	Microsoft::WRL::ComPtr<ID3D11Buffer> GetViewBuffer() const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetProjectionBuffer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_viewBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_projectionBuffer;
};