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
		start = chrono::high_resolution_clock::now();
	}
	
	
	unsigned long long elapsedNanoseconds() const
	{
		return chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();
	}


private:
	chrono::time_point<chrono::high_resolution_clock> start;
};

#endif
