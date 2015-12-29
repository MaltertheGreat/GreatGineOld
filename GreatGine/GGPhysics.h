#pragma once

class GGWorld;
class GGObject;

class GGPhysics
{
public:
	void Update( GGWorld& _world, double _frameTime );

private:
	void UpdateObject( GGObject& _object, double _frameTime );
};