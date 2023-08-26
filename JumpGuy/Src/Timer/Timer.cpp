#include "Timer.h"

Timer::Timer()
{
	m_time_passed = 0.0f;
	QueryPerformanceFrequency(&m_time_frequency);
}

void Timer::start()
{
	QueryPerformanceCounter(&m_time_start);
}

void Timer::end()
{
	QueryPerformanceCounter(&m_time_end);

	m_time_passed = ((float)m_time_end.QuadPart - (float)m_time_start.QuadPart) / m_time_frequency.QuadPart;
	
	if (m_time_passed > 0.15f) 
		m_time_passed = 0.15f;
}