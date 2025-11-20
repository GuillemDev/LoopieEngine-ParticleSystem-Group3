#include "Time.h"
#include <SDL3/SDL_timer.h>
namespace Loopie
{
	float Time::m_lastFrameTime = 0;
	int Time::m_frameCount = 0;
	float Time::m_fixedDeltaTime = 0.2f;
	float Time::m_timeScale = 1;
	float Time::m_deltaTime = 0;
	float Time::m_runTime = 0;
	float Time::m_executionTime = 0;

	void Time::CalculateFrame()
	{
		Uint64 now = SDL_GetPerformanceCounter();
		if (m_lastFrameTime != 0)
		{
			m_deltaTime = (float)((now - m_lastFrameTime) / (double)SDL_GetPerformanceFrequency());
		}
		else
		{
			m_deltaTime = 0.0f;
		}
		m_lastFrameTime = now;

		m_runTime += GetDeltaTime();
		m_executionTime += GetUnscaledDeltaTime();

		m_frameCount++;
	}

	void Time::SetFixedDeltaTimeMs(float value)
	{
		m_fixedDeltaTime = value * 0.001f;
	}

	void Time::SetFixedDeltaTime(float value)
	{
		m_fixedDeltaTime = value;
	}

	void Time::SetTimeScale(float value)
	{
		if (value < 0.0f)
			value = 0.0f;

		m_timeScale = value;
	}
}