#pragma once

class GGFPSCounter
{
public:
	void Update( float _frameTime );

	int GetFPS() const;

private:
	float m_fps = 0.0f;
	float m_frameTimeAccumulator = 0.0f;
	int m_measurementCount = 0;
};