namespace Loopie {
	class Time
	{
	public:

		Time() = delete;
		~Time() = delete;

		static void CalculateFrame();

		static void SetFixedDeltaTime(float seconds);
		static void SetFixedDeltaTimeMs(float ms);
		static void SetTimeScale(float scale);

		static float GetDeltaTime() { return m_deltaTime * m_timeScale; }
		static float GetDeltaTimeMs() { return m_deltaTime * 1000.f * m_timeScale; }

		static float GetFixedDeltaTime() { return m_fixedDeltaTime * m_timeScale; }
		static float GetFixedDeltaTimeMs() { return m_fixedDeltaTime * 1000.f * m_timeScale; }

		static float GetUnscaledDeltaTime() { return m_deltaTime; }
		static float GetUnscaledDeltaTimeMs() { return m_deltaTime * 1000.f; }

		static float GetUnscaledFixedDeltaTime() { return m_fixedDeltaTime; }
		static float GetUnscaledFixedDeltaTimeMs() { return m_fixedDeltaTime * 1000.f; }

		static float GetLastFrameTime() { return m_lastFrameTime; }

		static float GetRunTime() { return m_runTime; }
		static float GetRunTimeMs() { return m_runTime * 1000.f; }

		static float GetExecutionTime() { return m_executionTime; }
		static float GetExecutionTimeMs() { return m_executionTime * 1000.f; }

		static int GetFrameCount() { return m_frameCount; }
		static float GetTimeScale() { return m_timeScale; }

	private:
		static float m_lastFrameTime;
		static int m_frameCount;

		//Physic
		static float m_fixedDeltaTime;

		// Game Clock
		static float m_deltaTime;
		static float m_timeScale;
		static float m_runTime;

		//Real Time Clock
		static float m_executionTime;
	};
}