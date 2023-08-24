#include "../Includes.h"

void Timer::Start()
{
	QueryPerformanceCounter(&_timeStart);
}

void Timer::End()
{
	QueryPerformanceCounter(&_timeEnd);
	_timePassed = ((float)_timeEnd.QuadPart - (float)_timeStart.QuadPart) / _timeFrequency.QuadPart;
	if (_timePassed > 0.15f) _timePassed = 0.15f;
	_totalTimePassed += _timePassed;
}