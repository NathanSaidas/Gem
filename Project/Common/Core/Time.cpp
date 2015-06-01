#include "Time.h"
#include <memory>
#ifdef _WIN32
#include <Windows.h>
#endif



namespace Gem
{
	// -- Globals for Win32 Time implementation.
#ifdef _WIN32
	LARGE_INTEGER g_StartTime;
	LARGE_INTEGER g_StartDelta;
	LARGE_INTEGER g_Frequency;
#endif


	bool Time::s_Initialized = false;
	float Time::s_DeltaTime = 0.0f;
	float Time::s_CurrentTime = 0.0f;
	TimeStruct Time::s_CurrentTimeStruct = TimeStruct();

	Time::Time()
	{

	}

	Time::~Time()
	{

	}

	float Time::GetDeltaTime()
	{
#ifdef GEM_DEBUG
		//TODO(Nathan): Log Error about initialization.
		if (!s_Initialized)
		{
			return 0.0f;
		}
#endif
		return s_DeltaTime;
	}

	float Time::GetTime()
	{
#ifdef GEM_DEBUG
		//TODO(Nathan): Log Error about initialization.
		if (!s_Initialized)
		{
			return 0.0f;
		}
#endif
		return s_CurrentTime;
	}

	TimeStruct Time::GetTimeStruct()
	{
#ifdef GEM_DEBUG
		//TODO(Nathan): Log Error about initialization.
		if (!s_Initialized)
		{
			return TimeStruct();
		}
#endif
		return s_CurrentTimeStruct;
	}

	bool Time::Initialize()
	{
#ifdef _WIN32
		//Exit: Failed to get the performance frequency
		if (QueryPerformanceFrequency(&g_Frequency) == FALSE)
		{
			return false;
		}

		//Exit: Failed to get the performance counter.
		if (QueryPerformanceCounter(&g_StartTime) == FALSE)
		{
			return false;
		}
		g_StartDelta = g_StartTime;
		s_Initialized = true;
		return true;
#else
		static_assert(false,"Implement Time::Initialize")
		return false;
#endif
	}

	void Time::Update()
	{

		//Calculate Delta Time and Current Time
#ifdef _WIN32
		LARGE_INTEGER end;
		if (QueryPerformanceCounter(&end) == FALSE)
		{
			return;
		}

		float prevTime = s_CurrentTime;
		s_CurrentTime = (float)(end.QuadPart - g_StartTime.QuadPart) / (float)g_Frequency.QuadPart;
		s_DeltaTime = s_CurrentTime - prevTime;
#endif
		//Calculate the TimeStruct
		TimeStruct & time = s_CurrentTimeStruct;
		float seconds = s_CurrentTime;
		memset(&time, 0, sizeof(TimeStruct));

		//foreach 60 seconds add a minute.
		while (seconds > 60.0f)
		{
			seconds -= 60.0f;
			time.minutes++;
		}

		//set remaining seconds.
		time.seconds = (int)(seconds + 0.5f);

		//foreach 60 minutes add an hour.
		while (time.minutes > 60)
		{
			time.minutes -= 60;
			time.hours++;
		}
	}

	
}