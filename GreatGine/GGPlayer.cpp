#include "PCH.h"
#include "GGPlayer.h"
using namespace std;
using namespace DirectX;

GGPlayer::GGPlayer( GGChunk& _chunk )
{
	GGObject::GGVoxelArray voxels;
	voxels[ 0 ].element = 1;
	float bodyRadius = 1.0f;
	XMFLOAT3 position = { 0.0f, 10.0f, 0.0f };
	GGObject playerHead = GGObject( move( voxels ), bodyRadius, position );

	m_headObjectID = _chunk.AddObject( move( playerHead ) );
}

void GGPlayer::Update( GGChunk& _chunk, float _timeDelta )
{
	auto& head = _chunk.GetObjects().at( m_headObjectID );

	XMVECTOR velocity = XMVectorSet( 0.0f, 1.0f, 0.0f, 0.0f );
	XMVECTOR position = XMLoadFloat3( &head.GetPosition() );

	position += XMVectorScale( velocity, _timeDelta );

	XMFLOAT3 pos;
	XMStoreFloat3( &pos, position );

	_chunk.ModifyObject( m_headObjectID, pos );

	return;
}