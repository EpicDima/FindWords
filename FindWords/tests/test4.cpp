#include "../Timer.h"

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <immintrin.h>

using namespace std;





void copy1(uint64_t* inFirst, uint64_t* inLast, uint64_t* out)
{
	while (inFirst != inLast) {
        *out++ = *inFirst++;
    }
}

const uint64_t n = 100000000;
const uint64_t cycles = 1;

int main()
{
	Timer timer;
	
	int nSum = 4;
	float sum[nSum] = {0, 0, 0, 0};
	
	uint64_t *a;
	
	for (uint64_t cycle = 0; cycle < cycles; cycle++) {
		timer.reset();
		a = new uint64_t[n];
		for (uint64_t i = 0; i < n; i++) {
			a[i] = i;
		}
		delete [] a;
		sum[0] += timer.elapsedNanoseconds() / 1e6;
		
		timer.reset();
		a = (uint64_t*) malloc(sizeof(uint64_t) * n);
		for (uint64_t i = 0; i < n; i++) {
			a[i] = i;
		}
		free(a);
		sum[1] += timer.elapsedNanoseconds() / 1e6;
		
		timer.reset();
		a = (uint64_t*) _aligned_malloc(sizeof(uint64_t) * n, sizeof(uint64_t) * 8);
		for (uint64_t i = 0; i < n; i++) {
			a[i] = i;
		}
		_aligned_free(a);
		sum[2] += timer.elapsedNanoseconds() / 1e6;
		
		timer.reset();
		a = (uint64_t*) _mm_malloc(sizeof(uint64_t) * n, sizeof(uint64_t) * 8);
		for (uint64_t i = 0; i < n; i++) {
			a[i] = i;
		}
		_mm_free(a);
		sum[3] += timer.elapsedNanoseconds() / 1e6;
	}
	
	
	/*
	uint64_t *a = new uint64_t[n];
	for (uint64_t i = 0; i < n; i++) {
		a[i] = i + 1;
	}
	
	int nSum = 5;
	float sum[nSum] = {0, 0, 0, 0, 0};
	
	for (uint64_t cycle = 0; cycle < cycles; cycle++) {
		uint64_t *b = new uint64_t[n];
		timer.reset();
		for (uint64_t i = 0; i < n; i++) {
			b[i] = a[i];
		}
		sum[0] += timer.elapsedNanoseconds() / 1e6;
		delete[] b;
		
		uint64_t *b1 = new uint64_t[n];
		timer.reset();
		for (register uint64_t i = 0; i < n; i++) {
			b1[i] = a[i];
		}
		sum[1] += timer.elapsedNanoseconds() / 1e6;
		delete[] b1;
		
		
		uint64_t *c = new uint64_t[n];
		timer.reset();
		copy(a, a + n, c);
		sum[2] += timer.elapsedNanoseconds() / 1e6;
		delete[] c;
		
		uint64_t *d = new uint64_t[n];
		timer.reset();
		copy1(a, a + n, d);
		sum[3] += timer.elapsedNanoseconds() / 1e6;
		delete[] d;
		
		uint64_t *e = new uint64_t[n];
		timer.reset();
		memcpy(e, a, sizeof(uint64_t) * n);
		sum[4] += timer.elapsedNanoseconds() / 1e6;
		delete[] e;
	}
	*/
	
	cout << "Mean:\n";
	for (int i = 0; i < nSum; i++) {
		cout << sum[i] / cycles << endl;
	}
	cout << "\nSum:\n";
	for (int i = 0; i < nSum; i++) {
		cout << sum[i] << endl;
	}
	
	
	return 0;
}
