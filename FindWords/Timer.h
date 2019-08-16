#ifndef TIMER_H
#define TIMER_H


#include <chrono>



class Timer
{
public:
	Timer()
	{ 
		reset();
	}
	
	
	void reset()
	{
		start = std::chrono::high_resolution_clock::now();
	}
	
	
	unsigned long long elapsedNanoseconds() const
	{
		return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now() - start).count();
	}


private:
	std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

#endif
