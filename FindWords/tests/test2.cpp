#include <iostream>
#include <functional>
#include <vector>

#include "BitSetD2.h"
#include "BitSetD.h"
#include "../BitSet.h"
#include "../Timer.h"

using namespace std;


const unsigned int n = 50000;
const unsigned int size1 = 31;
const unsigned int size2 = 31;
const unsigned int size = size1 * size2;

bool t;
bool** b;

template<typename T>
void f1(T* a)
{
	for (unsigned int i = 0; i < n; i++) {
		a[i] = T(size);
	}
}

template<typename T>
void f2(T* a)
{
	T* c = new T[n];
	for (unsigned int i = 0; i < n; i++) {
		c[i] = T(a[i]);
	}
}

template<typename T>
void f3(T* a)
{
	for (unsigned int i = 0; i < n; i++) {
		a[i].set(size - 1, false);
	}
}

template<typename T>
void f4(T* a)
{
	for (unsigned int i = 0; i < n; i++) {
		t = a[i][size - 1];
	}
}

template<typename T>
void f5(T* a)
{
	for (unsigned int i = 1; i < n; i++) {
		a[i] |= a[i - 1];
	}
}

template<typename T>
void f6(T* a)
{
	for (unsigned int i = 0; i < n; i++) {
		a[i].isAllTrue();
		a[i].isAllFalse();
	}
}

template<typename T>
void f7(T* a)
{
	for (unsigned int i = 0; i < n; i++) {
		a[i] = ~a[i];
	}
}

template<typename T>
void f8(T* a)
{
	for (unsigned int i = 0; i < n; i++) {
		a[i] = T(size, b, size1, size2);
	}
}


template<typename T>
void f9(T* a)
{
	for (unsigned int i = 1; i < n; i++) {
		a[i - 1] = a[i - 1] | a[i];
	}
}


BitSetD2* bitsetD2 = new BitSetD2[n];
BitSetD* bitsetD = new BitSetD[n];
BitSet* bitset = new BitSet[n];
const unsigned int cycles = 30;

int main()
{
	b = new bool* [size1];
	for (unsigned int i = 0; i < size1; i++) {
		b[i] = new bool[size2];
		for (unsigned int j = 0; j < size2; j++) {
			b[i][j] = (j + 1) % 2 == 0;
		}
	}

	Timer timer;

	vector<function<void()>> fs = {[]() {f1(bitsetD2);}, []() {f1(bitsetD);}, []() {f1(bitset);},
		                           []() {f2(bitsetD2);}, []() {f2(bitsetD);}, []() {f2(bitset);},
								   []() {f3(bitsetD2);}, []() {f3(bitsetD);}, []() {f3(bitset);},
							       []() {f4(bitsetD2);}, []() {f4(bitsetD);}, []() {f4(bitset);},
								   []() {f5(bitsetD2);}, []() {f5(bitsetD);}, []() {f5(bitset);},
								   []() {f6(bitsetD2);}, []() {f6(bitsetD);}, []() {f6(bitset);},
								   []() {f7(bitsetD2);}, []() {f7(bitsetD);}, []() {f7(bitset);},
								   []() {f8(bitsetD2);}, []() {f8(bitsetD);}, []() {f8(bitset);},
								   []() {f9(bitsetD2);}, []() {f9(bitsetD);}, []() {f9(bitset);}};
	
	float *fsResults = new float[fs.size()];
	for (unsigned int i = 0; i < fs.size(); i++) {
		fsResults[i] = 0;
	}
	
	float bitsetD2Sum = 0;
	float bitsetDSum = 0;
	float bitsetSum = 0;
	
	for (unsigned int k = 0; k < cycles; k++) {
		for (unsigned int i = 0; i < fs.size(); i++) {
			timer.reset();
			fs[i]();
			fsResults[i] += timer.elapsedNanoseconds() / 1e9;
		}
	}
	
	unsigned int k = 1;
	for (unsigned int i = 0; i < fs.size(); i++) {
		if (i % 3 == 0) {
			bitsetD2Sum += fsResults[i] / cycles;
			cout << k << " BitSetD2: " << fsResults[i] / cycles << endl;
		} else if (i % 3 == 1){
			bitsetDSum += fsResults[i] / cycles;
			cout << k << " BitSetD:  " << fsResults[i] / cycles << endl;
		} else {
			bitsetSum += fsResults[i] / cycles;
			cout << k << " BitSet:   " << fsResults[i] / cycles << endl << endl;
		}
		if ((i + 1) % 3 == 0) {
			k++;
		}
	}

	cout << "\n--------------\n";
	cout << "BitSetD2Sum: " << bitsetD2Sum << endl;
	cout << "BitSetDSum:  " << bitsetDSum << endl;
	cout << "BitSetSum:   " << bitsetSum << endl;
    return 0;
}
