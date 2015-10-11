#include "PCH.h"
#include "GGFPSCounter.h"

void GGFPSCounter::Update( double _frameTime )
{
	const int accumulatorSize = 30;
	if( m_measurementCount < accumulatorSize )
	{
		m_frameTimeAccumulator += _frameTime;

		++m_measurementCount;
	}
	else
	{
		//m_fps = m_fps * 0.1f + ((1.0f * accumulatorSize) / m_frameTimeAccumulator) * 0.9f;
		m_fps = (1.0f * accumulatorSize) / m_frameTimeAccumulator;
		m_measurementCount = 0;
		m_frameTimeAccumulator = 0.0f;
	}

	return;
}

int GGFPSCounter::GetFPS() const
{
	return static_cast<int>(m_fps);
}