#pragma once

class GGFrameTimer
{
public:
	GGFrameTimer();

public:
	void Start();
	void Update();

	float GetFrameTime() const;

private:
	double m_frequency = 0.0;
	__int64 m_startTime = 0;
	float m_frameTime = 0.0f;
};