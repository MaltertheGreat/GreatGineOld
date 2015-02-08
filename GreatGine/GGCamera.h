#pragma once

#include <d3d11.h>
#include <atlbase.h>

class GGCamera
{
public:
	GGCamera( ID3D11Buffer* _viewBuffer, ID3D11Buffer* _projectionBuffer );
	GGCamera( const GGCamera& ) = delete;
	// Visual optimizes this, so move contr isn't even called, but without it error occures
	GGCamera( GGCamera&& _from );

public:

	ID3D11Buffer* GetViewBuffer() const;
	ID3D11Buffer* GetProjectionBuffer() const;

private:
	CComPtr<ID3D11Buffer> m_viewBuffer;
	CComPtr<ID3D11Buffer> m_projectionBuffer;
};