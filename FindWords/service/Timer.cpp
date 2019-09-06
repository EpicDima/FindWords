#include "Timer.h"


Timer::Timer()
{
    reset();
}


void Timer::reset()
{
    start = std::chrono::high_resolution_clock::now();
}


uint64_t Timer::elapsedNanoseconds()
{
    return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start)
        .count();
}