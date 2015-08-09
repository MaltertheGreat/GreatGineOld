#pragma once

#include "GGInputHandler.h"
#include "GGWorld.h"

class GGInput;
class GGConfig;

class GGPlayer : public GGInputHandler
{
private:
	enum GG_KEYMAP
	{
		GG_KEYMAP_FORWARD,
		GG_KEYMAP_BACKWARD,
		GG_KEYMAP_RIGHTWARD,
		GG_KEYMAP_LEFTWARD,
		GG_KEYMAP_UPWARD,
		GG_KEYMAP_DOWNWARD,
		GG_KEYMAP_DIG,
		GG_KEYMAP_PLACE,

		GG_KEYMAP_COUNT
	};

public:
	GGPlayer( GGInput& _input, GGConfig& _config );

public:
	void Update( GGWorld& _world, float _timeDelta );

	const DirectX::XMFLOAT3 GetPosition() const;
	const DirectX::XMFLOAT3& GetRotation() const;

public:
	virtual void HandleKeyInput( WPARAM _keyCode, bool _down ) override;
	virtual void HandleMouseInput( int _x, int _y ) override;

private:
	static GGObject PlayerObject( const DirectX::XMFLOAT3& _pos );
	void UpdatePosition( GGWorld& _world, float _timeDelta );
	void InteractWithWorld( GGWorld& _world, float _timeDelta );

private:
	bool m_isAlive;
	bool m_digging;
	bool m_placing;
	float m_diggingCooldown;
	float m_placingCooldown;

	WPARAM m_keyMap[ GG_KEYMAP_COUNT ];
	GGChunk::GGObjectID m_headObjectID;

	UINT m_chunkX;
	UINT m_chunkZ;

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_velocity;
	DirectX::XMFLOAT3 m_rotation;
};