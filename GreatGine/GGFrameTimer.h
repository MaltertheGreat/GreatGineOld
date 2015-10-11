#pragma once

class GGFrameTimer
{
public:
	GGFrameTimer();

public:
	void Start();
	void Update();

	double GetFrameTime() const;

private:
	double m_frequency = 0.0;
	long long m_startTime = 0;
	double m_frameTime = 0.0;
};