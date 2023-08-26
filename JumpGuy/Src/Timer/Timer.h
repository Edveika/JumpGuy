#pragma once
#include <Windows.h>
#include <chrono>

class Timer
{
private:
	LARGE_INTEGER m_time_start;
	LARGE_INTEGER m_time_end;
	LARGE_INTEGER m_time_frequency;
	float m_time_passed;

public:
	Timer();

	void start();
	void end();

	// Getters
	float get_time_passed() { return m_time_passed; }
};