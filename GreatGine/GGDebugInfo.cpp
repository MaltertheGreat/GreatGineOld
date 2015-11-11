#include "PCH.h"
#include "GGDebugInfo.h"
using namespace DirectX;
using namespace std;

void GGDebugInfo::Update( double _timeDelta, const XMFLOAT3& _cameraPos, const DirectX::XMFLOAT3& _cameraRot )
{
	m_fpsCounter.Update( _timeDelta );

	m_lines[ 0 ] = L"FPS: ";
	m_lines[ 0 ] += to_wstring( m_fpsCounter.GetFPS() );

	m_lines[ 1 ] = L"X/Y/Z: ";
	m_lines[ 1 ] += to_wstring( _cameraPos.x );
	m_lines[ 1 ] += L" / ";
	m_lines[ 1 ] += to_wstring( _cameraPos.y );
	m_lines[ 1 ] += L" / ";
	m_lines[ 1 ] += to_wstring( _cameraPos.z );

	m_lines[ 2 ] = L"Roll/Pitch/Yaw ";
	m_lines[ 2 ] += to_wstring( XMConvertToDegrees( _cameraRot.x ) );
	m_lines[ 2 ] += L" / ";
	m_lines[ 2 ] += to_wstring( XMConvertToDegrees( _cameraRot.y ) );
	m_lines[ 2 ] += L" / ";
	m_lines[ 2 ] += to_wstring( XMConvertToDegrees( _cameraRot.z ) );

	return;
}

const wstring& GGDebugInfo::GetLine( int _lineNumber ) const
{
	return m_lines[ _lineNumber ];
}