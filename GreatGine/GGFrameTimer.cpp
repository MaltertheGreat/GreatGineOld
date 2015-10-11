#include "PCH.h"
#include "GGFrameTimer.h"
#include "GGError.h"

GGFrameTimer::GGFrameTimer()
{
	LARGE_INTEGER frequency;
	if( !QueryPerformanceFrequency( &frequency ) )
	{
		GG_THROW;
	}

	m_frequency = static_cast<double>(frequency.QuadPart);
}

void GGFrameTimer::Start()
{
	LARGE_INTEGER count;
	if( !QueryPerformanceCounter( &count ) )
	{
		GG_THROW;
	}

	m_startTime = count.QuadPart;

	return;
}

void GGFrameTimer::Update()
{
	LARGE_INTEGER count;
	if( !QueryPerformanceCounter( &count ) )
	{
		GG_THROW;
	}

	long long currentTime = count.QuadPart;

	m_frameTime = (currentTime - m_startTime) / m_frequency;
	m_startTime = currentTime;

	return;
}

double GGFrameTimer::GetFrameTime() const
{
	return m_frameTime;
}