#pragma once

#include "GGFPSCounter.h"
#include "PCH.h"

class GGDebugInfo
{
public:
	static const int LINE_COUNT = 3;
public:
	void Update( double _frameTime, const DirectX::XMFLOAT3& _cameraPos, const DirectX::XMFLOAT3& _cameraRot );

	const std::wstring& GetLine( int _lineNumber ) const;

private:
	GGFPSCounter m_fpsCounter;
	std::wstring m_lines[ LINE_COUNT ];
};