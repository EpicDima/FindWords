#include <iostream>

#include "../Timer.h"

using namespace std;

typedef unsigned long long ull_t;


const ull_t TYPESIZE_IN_BITS = 64;
const ull_t n = 10000000;
const ull_t length = n / TYPESIZE_IN_BITS + 1;

ull_t data[length];

pair<ull_t, ull_t> f0(ull_t index) 
{
	return make_pair(index / TYPESIZE_IN_BITS, 1ULL << (index % TYPESIZE_IN_BITS));
}

bool f00(ull_t index)
{
	pair<ull_t, ull_t> info = f0(index);
	return (data[info.first] & info.second) != 0;
}

struct Strct 
{
	ull_t v1;
	ull_t v2;
};

Strct f1(ull_t index) 
{
	return {index / TYPESIZE_IN_BITS, 1ULL << (index % TYPESIZE_IN_BITS)};
}


bool f11(ull_t index)
{
	Strct info = f1(index);
	return (data[info.v1] & info.v2) != 0;
}


void f111(ull_t index, bool value) 
{
	Strct info = f1(index);
	if (value) {
		data[info.v1] |= info.v2;
	} else {
		data[info.v1] &= ~info.v2;
	}
}


ull_t* f2(ull_t index) 
{
	ull_t* t = new ull_t[2];
	t[0] = index / TYPESIZE_IN_BITS;
	t[1] = 1ULL << (index % TYPESIZE_IN_BITS);
	return t;
}

bool f22(ull_t index)
{
	ull_t* t = f2(index);
	return (data[t[0]] & t[1]) != 0;
}



void f3(ull_t index, ull_t& typeindex, ull_t& mask) 
{
	typeindex = index / TYPESIZE_IN_BITS;
	mask = 1ULL << (index % TYPESIZE_IN_BITS);
}


bool f33(ull_t index)
{
	ull_t v1, v2;
	f3(index, v1, v2);
	return (data[v1] & v2) != 0;
}

void f333(ull_t index, bool value) 
{
	ull_t v1, v2;
	f3(index, v1, v2);
	if (value) {
		data[v1] |= v2;
	} else {
		data[v1] &= ~v2;
	}
}


ull_t f4(ull_t index, ull_t& typeindex) 
{
	typeindex = index / TYPESIZE_IN_BITS;
	return 1ULL << (index % TYPESIZE_IN_BITS);
}


bool f44(ull_t index)
{
	ull_t v1, v2;
	v2 = f4(index, v1);
	return (data[v1] & v2) != 0;
}

void f444(ull_t index, bool value) 
{
	ull_t v1, v2;
	v2 = f4(index, v1);
	if (value) {
		data[v1] |= v2;
	} else {
		data[v1] &= ~v2;
	}
}


ull_t f5(ull_t index, ull_t& mask) 
{
	mask = 1ULL << (index % TYPESIZE_IN_BITS);
	return index / TYPESIZE_IN_BITS;
}


bool f55(ull_t index)
{
	ull_t v2;
	return (data[f5(index, v2)] & v2) != 0;
}

void f555(ull_t index, bool value) 
{
	ull_t v2;
	if (value) {
		data[f5(index, v2)] |= v2;
	} else {
		data[f5(index, v2)] &= ~v2;
	}
}


ull_t& f6(ull_t index, ull_t& mask) 
{
	mask = 1ULL << (index % TYPESIZE_IN_BITS);
	return data[index / TYPESIZE_IN_BITS];
}


bool f66(ull_t index)
{
	ull_t v2;
	return (f6(index, v2) & v2) != 0;
}


void f666(ull_t index, bool value) 
{
	ull_t v2;
	ull_t& v1 = f6(index, v2);
	v1 = value ? (v1 | v2) : (v1 & ~v2);
}


ull_t& test()
{
	data[4] = true;
	return data[4];
}


void ft1()
{
	bool cond;
	for (unsigned int i = 1; i < length; i++) {
		bool cond = i % 2 == 0;
		if (cond) {
			data[i] |= data[i - 1];
		} else {
			data[i] &= ~data[i - 1];
		}
	}
}


void ft2()
{
	for (unsigned int i = 1; i < length; i++) {
		bool cond = i % 2 == 0;
		ull_t t1 = data[i] | data[i - 1];
		ull_t t2 = data[i] & ~data[i - 1];
		data[i] = cond ? t1 : t2;
	}
}

void ft3()
{
	for (unsigned int i = 1; i < length; i++) {
		bool cond = i % 2 == 0;
		ull_t cond2 = static_cast<ull_t>(cond);
		cond2++;
		data[i] = (~cond2 & (data[i] | data[i - 1])) | (cond2 & (data[i] & ~data[i - 1]));
	}
}

static __inline__ unsigned long long rdtsc(void)
{
    unsigned hi, lo;
    __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
    return ( (unsigned long long)lo)|( ((unsigned long long)hi)<<32 );
}


int main()
{
	Timer timer;
	bool bl;
	
//	timer.reset();
//	for (unsigned int i = 0; i < n; i++)
//	{
//		bl = f00(i);
//	}
//	cout << timer.elapsedNanoseconds() << endl;
	
	timer.reset();
	for (unsigned int i = 0; i < n; i++)
	{
		bl = f11(i);
		f111(i, bl);
	}
	cout << timer.elapsedNanoseconds() << endl;
	
//	timer.reset();
//	for (unsigned int i = 0; i < n; i++)
//	{
//		bl = f22(i);
//	}
//	cout << timer.elapsedNanoseconds() << endl;
	
	timer.reset();
	for (unsigned int i = 0; i < n; i++)
	{
		bl = f33(i);
		f333(i, bl);
	}
	cout << timer.elapsedNanoseconds() << endl;
	
	timer.reset();
	for (unsigned int i = 0; i < n; i++)
	{
		bl = f44(i);
		f444(i, bl);
	}
	cout << timer.elapsedNanoseconds() << endl;
	
	
	timer.reset();
	for (unsigned int i = 0; i < n; i++)
	{
		bl = f55(i);
		f555(i, bl);
	}
	cout << timer.elapsedNanoseconds() << endl;
	
	timer.reset();
	for (unsigned int i = 0; i < n; i++)
	{
		bl = f66(i);
		f666(i, bl);
	}
	cout << timer.elapsedNanoseconds() << endl;
	
	unsigned long long cycles1 = rdtsc();
	unsigned long long cycles2 = rdtsc();
	cout << "cycles: " << cycles2 - cycles1 << endl;
	
	/*
	timer.reset();
	ft1();
	cout << timer.elapsedNanoseconds() << endl;
	timer.reset();
	ft2();
	cout << timer.elapsedNanoseconds() << endl;
	timer.reset();
	ft3();
	cout << timer.elapsedNanoseconds() << endl;
	*/

	return 0;
}
