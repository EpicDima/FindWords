#ifndef BITSETD2_H
#define BITSETD2_H


#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;


typedef unsigned long long ull_t;


const ull_t TYPESIZE_IN_BYTES = sizeof(ull_t);
const ull_t TYPESIZE_IN_BITS = TYPESIZE_IN_BYTES * 8;
const ull_t ULL_MAX_VALUE = ~0ULL;


class BitSetD2
{
public:
	BitSetD2() {}
	
	
	BitSetD2(ull_t nBits) : nBits(nBits)
	{
		count = nBits / TYPESIZE_IN_BITS;
		ull_t difference = nBits % TYPESIZE_IN_BITS;
		if (difference != 0) {
			count++;
			lastmask = ~(~lastmask >> (TYPESIZE_IN_BITS - difference));
		}
		data = reinterpret_cast<ull_t*>(memset(new ull_t[count], 0, count * TYPESIZE_IN_BYTES));
	}
	
	
	BitSetD2(ull_t nBits, bool** a, ull_t n, ull_t m) : BitSetD2(nBits)
	{
		ull_t mask = 1;
		ull_t k = 0;
		for (ull_t i = 0; i < n; i++) {
			for (ull_t j = 0; j < m; j++) {
				if (a[i][j]) {
					data[k] |= mask;
				} else {
					data[k] &= ~mask;
				}
				mask <<= 1;
				if (mask == 0) {
					k++;
					mask = 1;
				}
			}
		}
	}
	

	BitSetD2(const BitSetD2& b) : nBits(b.nBits), count(b.count), lastmask(b.lastmask)
	{
		this->data = new ull_t[this->count];
		
		// оптимизация под небольшие массивы, такие как обычно в FindWords
		for (ull_t i = 0; i < this->count; i++) {
			this->data[i] = b.data[i];
		}
		
		// для общего использования
		//memcpy(this->data, b.data, b.count * TYPESIZE_IN_BYTES);
	}
	
	
	~BitSetD2()
	{
		delete [] data;
	}
	
	
	BitSetD2& operator&= (const BitSetD2& b)
	{
		assert(this->nBits == b.nBits);
		for (ull_t i = 0; i < this->count; i++) {
			this->data[i] &= b.data[i];
		}
		return *this;
	}
	
	
	BitSetD2& operator|= (const BitSetD2& b)
	{
		assert(this->nBits == b.nBits);
		for (ull_t i = 0; i < this->count; i++) {
			this->data[i] |= b.data[i];
		}
		return *this;
	}
	
	
	BitSetD2 operator& (const BitSetD2& b)
	{
		BitSetD2 temp(*this);
		temp &= b;
		return temp;
	}
	
	
	BitSetD2 operator| (const BitSetD2& b)
	{
		BitSetD2 temp(*this);
		temp |= b;
		return temp;
	}
	
	
	BitSetD2 operator~ ()
	{
		BitSetD2 temp(*this);
		for (ull_t i = 0; i < temp.count; i++) {
			temp.data[i] = ~temp.data[i];
		}
		temp.data[temp.count - 1] = ~temp.lastmask & temp.data[temp.count - 1];
		return temp;
	}
	
	
	bool operator[] (ull_t index)
	{
		ull_t mask;
		return (data[getTypeIndexAndMask(index, mask)] & mask) != 0;
	}
	
	
	void set(ull_t index, bool value) 
	{
		ull_t mask;
		if (value) {
			data[getTypeIndexAndMask(index, mask)] |= mask;
		} else {
			data[getTypeIndexAndMask(index, mask)] &= ~mask;
		}
	}
	
	
    bool isAllTrue()
    {
    	for (ull_t i = 0; i < count - 1; i++) {
			if (data[i] != ULL_MAX_VALUE) {
				return false;
			}
		}
		return (data[count - 1] | lastmask) == ULL_MAX_VALUE;
	}
	
	
	bool isAllFalse()
    {
    	for (ull_t i = 0; i < count; i++) {
			if (data[i] != 0) {
				return false;
			}
		}
		return true;
	}
	
	
private:
	ull_t nBits;
	ull_t count;
	ull_t lastmask = 0;
	ull_t* data;


	ull_t getTypeIndexAndMask(ull_t index, ull_t& mask)
	{
		assert(index < nBits);
		mask = 1ULL << (index % TYPESIZE_IN_BITS);
		return index / TYPESIZE_IN_BITS;
	}
};

#endif
