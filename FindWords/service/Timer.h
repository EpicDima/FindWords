#ifndef TIMER_H
#define TIMER_H


#include <chrono>


class Timer
{
public:
    Timer();
    void reset();
    uint64_t elapsedNanoseconds();

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

#endif
