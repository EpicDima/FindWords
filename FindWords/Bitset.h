#ifndef BITSET_H
#define BITSET_H


#include <iostream>
#include <cassert>

using namespace std;


typedef unsigned long long ull_t;
const unsigned int TYPESIZE = 64;


class BitSet
{
public:
	BitSet(unsigned int nBits)
	{
		this->nBits = nBits; 
		count = nBits / TYPESIZE;
		lastmask = 0;
		unsigned int difference = nBits % TYPESIZE;
		if (difference != 0) {
			count++;
			lastmask = ~((~lastmask) >> (TYPESIZE - difference));
		}
		data = new ull_t[count];
		for (unsigned int i = 0; i < count; i++) {
			data[i] = 0;
		}
	}
	
	
	BitSet(unsigned int nBits, bool** a, unsigned int n, unsigned int m) : BitSet(nBits)
	{
		ull_t mask = 1;
		unsigned int k = 0;
		for (unsigned int i = 0; i < n; i++) {
			for (unsigned int j = 0; j < m; j++) {
				unsigned int t = i * m + j;
				if (t % TYPESIZE == 0 && t != 0) {
					k++;
					assert(k < count);
					mask = 1;
				}
				if (a[i][j]) {
					data[k] |= mask;
				} else {
					data[k] &= ~mask;
				}
				mask <<= 1;
			}
		}
	}
	

	BitSet(const BitSet& b) : BitSet(b.nBits)
	{
		for (unsigned int i = 0; i < count; i++) {
			this->data[i] = b.data[i];
		}
	}
	
	
	~BitSet()
	{
		delete[] data;
	}
	
	
	BitSet& operator&= (const BitSet& b) 
	{
		assert(this->nBits == b.nBits);
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] &= b.data[i];
		}
		return *this;
	}
	
	
	BitSet& operator|= (const BitSet& b) 
	{
		assert(this->nBits == b.nBits);
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] |= b.data[i];
		}
		return *this;
	}
	
	
	BitSet operator& (const BitSet& b) 
	{
		BitSet temp(*this);
		temp &= b;
		return temp;
	}
	
	
	BitSet operator| (const BitSet& b)
	{
		BitSet temp(*this);
		temp |= b;
		return temp;
	}
	
	
	BitSet operator~ ()
	{
		BitSet temp(*this);
		for (unsigned int i = 0; i < temp.count; i++) {
			temp.data[i] = ~temp.data[i];
		}
		temp.data[temp.count - 1] = temp.data[temp.count - 1] & ~temp.lastmask;
		return temp;
	}
	
	
	bool operator[] (const unsigned int index) const
	{
		pair<unsigned int, ull_t> p = getMaskByIndex(index);
		return (data[p.first] & p.second) != 0;
	}
	
	
	void set(const unsigned int index, const bool value) 
	{
		pair<unsigned int, ull_t> p = getMaskByIndex(index);
		if (value) {
			data[p.first] |= p.second;
		} else {
			data[p.first] &= ~p.second;
		}
	}
	
	
    bool isAllTrue() const
    {
    	for (unsigned int i = 0; i < (count - 1); i++) {
			if (~data[i] != 0) {
				return false;
			}
		}
		return ~(data[count - 1] | lastmask) == 0;
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
	ull_t* data;
	ull_t lastmask;


	pair<unsigned int, ull_t> getMaskByIndex(const unsigned int index) const
	{
		unsigned int typeindex = index / TYPESIZE;
		if (typeindex >= count) {
			cout << "\n--->>> " << index << "\t" << typeindex << "\t" << count << "\t" << nBits << endl;
		}
		assert(typeindex < count);
		ull_t mask = 1;
		mask <<= (index % TYPESIZE);
		return make_pair(typeindex, mask);
	}
};

#endif
