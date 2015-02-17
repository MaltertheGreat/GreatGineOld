#pragma once

#include "GGInputProcessor.h"
#include "GGIWorldViewer.h"
#include "GGFreeCamera.h"

class GGWorld
{
public:
	GGWorld( GGInputProcessor& _inputProcessor );

public:
	void Update( float _frameTime );

	const GGIWorldViewer* GetActiveWorldViewer() const;

private:
	GGFreeCamera m_freeCamera;
	// GGPlayerCamera m_playerCamera;
};