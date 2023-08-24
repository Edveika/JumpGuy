#pragma once
#include <Windows.h>
#include <chrono>

class Timer
{
private:
	LARGE_INTEGER _timeStart;
	LARGE_INTEGER _timeEnd;
	LARGE_INTEGER _timeFrequency;
	float _timePassed;
	float _totalTimePassed;

public:
	Timer()
	{
		_timePassed = 0.0f;
		QueryPerformanceFrequency(&_timeFrequency);
	}

public:
	void Start();
	void End();

	// Getters
	LONGLONG GetTimeStart() { return _timeStart.QuadPart; }
	LONGLONG GetTimeEnd() { return _timeEnd.QuadPart; }
	LONGLONG GetTimeFreq() { return _timeFrequency.QuadPart; }
	float GetTimePassed() { return _timePassed; }
	float GetTotalTimePassed() { return _totalTimePassed; }
};