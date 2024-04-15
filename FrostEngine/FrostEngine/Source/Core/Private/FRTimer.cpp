#include "FRTimer.h"

using namespace std::chrono;

FRTimer::FRTimer()
{
	last = steady_clock::now();
}

float FRTimer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float FRTimer::Peek()
{
	return duration<float>(steady_clock::now() - last).count();

}

