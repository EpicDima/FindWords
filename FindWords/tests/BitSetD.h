#ifndef BITSETD_H
#define BITSETD_H


#include <iostream>
#include <cassert>
#include <cstring>

using namespace std;


typedef unsigned long long ull_t;
//const unsigned int TYPESIZE_IN_BYTES = sizeof(ull_t);
//const unsigned int TYPESIZE_IN_BITS = TYPESIZE_IN_BYTES * 8;
//const ull_t ULL_MAX_VALUE = -1;


class BitSetD
{
public:
	BitSetD() {}
	
	
	BitSetD(unsigned int nBits)
	{
		this->nBits = nBits; 
		count = nBits / TYPESIZE_IN_BITS;
		unsigned int difference = nBits % TYPESIZE_IN_BITS;
		if (difference != 0) {
			count++;
			lastmask = ~(~lastmask >> (TYPESIZE_IN_BITS - difference));
		}
		data = reinterpret_cast<ull_t*>(memset(new ull_t[count], 0, TYPESIZE_IN_BYTES * count));
	}
	
	
	BitSetD(unsigned int nBits, bool** a, unsigned int n, unsigned int m) : BitSetD(nBits)
	{
		ull_t mask = 1;
		unsigned int k = 0;
		for (unsigned int i = 0; i < n; i++) {
			for (unsigned int j = 0; j < m; j++) {
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
	

	BitSetD(const BitSetD& b) : BitSetD(b.nBits)
	{
		copy(b.data, b.data + b.count, this->data);
	}
	
	
	~BitSetD()
	{
		delete[] data;
	}
	
	
	BitSetD& operator&= (const BitSetD& b) 
	{
		assert(this->nBits == b.nBits);
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] &= b.data[i];
		}
		return *this;
	}
	
	
	BitSetD& operator|= (const BitSetD& b) 
	{
		assert(this->nBits == b.nBits);
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] |= b.data[i];
		}
		return *this;
	}
	
	
	BitSetD operator& (const BitSetD& b) 
	{
		BitSetD temp(*this);
		temp &= b;
		return temp;
	}
	
	
	BitSetD operator| (const BitSetD& b)
	{
		BitSetD temp(*this);
		temp |= b;
		return temp;
	}
	
	
	BitSetD operator~ ()
	{
		BitSetD temp(*this);
		for (unsigned int i = 0; i < temp.count; i++) {
			temp.data[i] = ~temp.data[i];
		}
		temp.data[temp.count - 1] = ~temp.lastmask & temp.data[temp.count - 1];
		return temp;
	}
	
	
	bool operator[] (const unsigned int index) const
	{
		IndexInfo info = getIndexInfo(index);
		return (data[info.typeindex] & info.mask) != 0;
	}
	
	
	void set(const unsigned int index, const bool value) 
	{
		IndexInfo info = getIndexInfo(index);
		if (value) {
			data[info.typeindex] |= info.mask;
		} else {
			data[info.typeindex] &= ~info.mask;
		}
	}
	
	
    bool isAllTrue() const
    {
    	for (unsigned int i = 0; i < count - 1; i++) {
			if (data[i] != ULL_MAX_VALUE) {
				return false;
			}
		}
		return (data[count - 1] | lastmask) == ULL_MAX_VALUE;
	}
	
	
	bool isAllFalse() const
    {
    	for (unsigned int i = 0; i < count; i++) {
			if (data[i] != 0) {
				return false;
			}
		}
		return true;
	}
	
	
private:
	unsigned int nBits;
	unsigned int count;
	ull_t lastmask = 0;
	ull_t* data;
	

	struct IndexInfo
	{
		unsigned int typeindex;
		ull_t mask;
	};
	

	IndexInfo getIndexInfo(const unsigned int index) const
	{
		assert(index < nBits);
		return {index / TYPESIZE_IN_BITS, 1ULL << (index % TYPESIZE_IN_BITS)};
	}
};

#endif
