#include "PCH.h"
#include "GGWorld.h"

GGWorld::GGWorld( GGInputProcessor& _inputProcessor )
{
	_inputProcessor.RegisterHandler( &m_freeCamera );
}

void GGWorld::Update( float _frameTime )
{
	m_freeCamera.Update( _frameTime );

	return;
}

const GGIWorldViewer* GGWorld::GetActiveWorldViewer() const
{
	return dynamic_cast<const GGIWorldViewer*>(&m_freeCamera);
}