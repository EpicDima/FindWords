#include <iostream>
#include <chrono>

using namespace std;


class Timer
{
public:
	Timer() { reset(); } 
	void reset() { start = chrono::high_resolution_clock::now(); }
	unsigned long long elapsedNanoseconds() const
	{
		return chrono::duration_cast<chrono::nanoseconds>(chrono::high_resolution_clock::now() - start).count();
	}
	
private:
	chrono::time_point<chrono::high_resolution_clock> start;
};


int main()
{
	unsigned int t = 123;
	Timer timer;
	for (unsigned int i = 0; i < 100000000; i++)
	{
		t += i;
		t = i;
	}
	
	unsigned long long calculationTimeInNanoseconds = timer.elapsedNanoseconds();
	cout << "Nanoseconds:   " << calculationTimeInNanoseconds << endl << endl;
	cout << "Microseconds:  " << static_cast<double>(calculationTimeInNanoseconds / 1000.0) << endl << endl;
	cout << "Milliseconds:  " << static_cast<double>(calculationTimeInNanoseconds / 1000000.0) << endl << endl;
	
	return 0;
}
