#pragma once

#include "GGFPSCounter.h"
#include "PCH.h"

class GGDebugInfo
{
public:
	static const int LINE_COUNT = 2;
public:
	void Update( float _frameTime, const DirectX::XMFLOAT3& _cameraPos );

	const std::wstring& GetLine( int _lineNumber ) const;

private:
	GGFPSCounter m_fpsCounter;
	std::wstring m_lines[ LINE_COUNT ];
};