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
	double m_frequency;
	__int64 m_startTime;
	float m_frameTime;
};