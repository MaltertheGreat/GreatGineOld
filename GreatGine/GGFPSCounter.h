#pragma once

class GGFPSCounter
{
public:
	void Update( double _frameTime );

	int GetFPS() const;

private:
	double m_fps = 0.0f;
	double m_frameTimeAccumulator = 0.0f;
	int m_measurementCount = 0;
};