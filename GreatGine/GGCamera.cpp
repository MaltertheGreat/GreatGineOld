#include "PCH.h"
#include "GGCamera.h"
using Microsoft::WRL::ComPtr;
using namespace std;

GGCamera::GGCamera( ComPtr<ID3D11Buffer> _viewBuffer, ComPtr<ID3D11Buffer> _projectionBuffer )
	:
	m_viewBuffer( _viewBuffer ),
	m_projectionBuffer( _projectionBuffer )
{}

/*GGCamera::GGCamera( GGCamera&& _from )
{
m_viewBuffer.Attach( _from.m_viewBuffer.Detach() );
m_projectionBuffer.Attach( _from.m_projectionBuffer.Detach() );
}*/

ComPtr<ID3D11Buffer> GGCamera::GetViewBuffer() const
{
	return m_viewBuffer;
}

ComPtr<ID3D11Buffer> GGCamera::GetProjectionBuffer() const
{
	return m_projectionBuffer;
}