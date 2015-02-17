#pragma once

#include "GGIWorldViewer.h"
#include "GGInputHandler.h"

class GGFreeCamera : public GGInputHandler, public GGIWorldViewer
{
public:
	GGFreeCamera( const DirectX::XMFLOAT3& _position = { 0.0f, 0.0f, 0.0f } );

public:
	void Update( float _frameTime );

public:
	virtual void HandleActionInput( GG_ACTION_INPUT _input, bool _down ) override;
	virtual void HandleRangeInput( int _x, int _y ) override;

	virtual const DirectX::XMFLOAT3& GetPosition() const override;
	virtual const DirectX::XMFLOAT3& GetRotation() const override;

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_rotation;
};