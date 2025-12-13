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

		static float GetDeltaTime() { return s_DeltaTime * s_TimeScale; }
		static float GetDeltaTimeMs() { return s_DeltaTime * 1000.f * s_TimeScale; }

		static double GetFixedDeltaTime() { return s_FixedDeltaTime * s_TimeScale; }
		static double GetFixedDeltaTimeMs() { return s_FixedDeltaTime * 1000.f * s_TimeScale; }

		static double GetUnscaledDeltaTime() { return s_DeltaTime; }
		static double GetUnscaledDeltaTimeMs() { return s_DeltaTime * 1000.f; }

		static float GetUnscaledFixedDeltaTime() { return s_FixedDeltaTime; }
		static float GetUnscaledFixedDeltaTimeMs() { return s_FixedDeltaTime * 1000.f; }

		static float GetLastFrameTime() { return s_LastFrameTime; }

		static float GetRunTime() { return s_RunTime; }
		static float GetRunTimeMs() { return s_RunTime * 1000.f; }

		static float GetExecutionTime() { return s_ExecutionTime; }
		static float GetExecutionTimeMs() { return s_ExecutionTime * 1000.f; }

		static int GetFrameCount() { return s_FrameCount; }
		static float GetTimeScale() { return s_TimeScale; }

	private:
		static double s_LastFrameTime;
		static int s_FrameCount;

		//Physic
		static double s_FixedDeltaTime;

		// Game Clock
		static double s_DeltaTime;
		static float s_TimeScale;
		static float s_RunTime;

		//Real Time Clock
		static float s_ExecutionTime;
	};
}