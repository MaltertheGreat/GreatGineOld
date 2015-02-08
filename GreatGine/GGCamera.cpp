#include "GGCamera.h"
#include <string>
using namespace std;

GGCamera::GGCamera( ID3D11Buffer* _viewBuffer, ID3D11Buffer* _projectionBuffer )
	:
	m_viewBuffer( _viewBuffer ),
	m_projectionBuffer( _projectionBuffer )
{}

GGCamera::GGCamera( GGCamera&& _from )
	:
	m_viewBuffer( _from.m_viewBuffer ),
	m_projectionBuffer( _from.m_projectionBuffer )
{}

ID3D11Buffer* GGCamera::GetViewBuffer() const
{
	return m_viewBuffer.p;
}

ID3D11Buffer* GGCamera::GetProjectionBuffer() const
{
	return m_projectionBuffer.p;
}