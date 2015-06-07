#pragma once

#include "GGIWorldViewer.h"
#include "GGInputHandler.h"

class GGConfig;

class GGFreeCamera : public GGInputHandler, public GGIWorldViewer
{
	enum GG_KEYMAP
	{
		GG_KEYMAP_FORWARD,
		GG_KEYMAP_BACKWARD,
		GG_KEYMAP_RIGHTWARD,
		GG_KEYMAP_LEFTWARD,
		GG_KEYMAP_UPWARD,
		GG_KEYMAP_DOWNWARD,

		GG_KEYMAP_COUNT
	};
public:
	explicit GGFreeCamera( const DirectX::XMFLOAT3& _position, GGConfig& _config );

public:
	void Update( float _frameTime );

public:
	virtual void HandleKeyInput( WPARAM _keyCode, bool _down ) override;
	virtual void HandleMouseInput( int _x, int _y ) override;

	virtual const DirectX::XMFLOAT3& GetPosition() const override;
	virtual const DirectX::XMFLOAT3& GetRotation() const override;

private:
	WPARAM m_keyMap[ GG_KEYMAP_COUNT ];
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_rotation;
};