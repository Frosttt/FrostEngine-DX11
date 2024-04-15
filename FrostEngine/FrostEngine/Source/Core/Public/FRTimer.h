#pragma once

#include <chrono>

class FRTimer
{
public:
	FRTimer();
	float Mark();
	float Peek();
private:
	std::chrono::steady_clock::time_point last;
};

