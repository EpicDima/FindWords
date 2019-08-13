#ifndef BITSET_H
#define BITSET_H


#include <iostream>
#include <cassert>

using namespace std;



template <typename T>
class Bitset
{
public:
	Bitset(unsigned int nBits)
	{
		this->nBits = nBits; 
		unsigned int typesize = sizeof(T) * 8;
		count = static_cast<unsigned int>(nBits / typesize);
		lastmask = 0;
		unsigned int difference = nBits % typesize;
		if (difference != 0) {
			count++;
			lastmask = ~((~lastmask) >> (typesize - difference));
		}
		data = new T[count];
		for (unsigned int i = 0; i < count; i++) {
			data[i] = 0;
		}
	}
	
	
	Bitset(unsigned int nBits, bool** a, unsigned int n, unsigned int m) : Bitset(nBits)
	{
		unsigned int typesize = sizeof(T) * 8;
		T mask = 1;
		unsigned int k = 0;
		for (unsigned int i = 0; i < n; i++) {
			for (unsigned int j = 0; j < m; j++) {
				unsigned int t = i * n + j;
				if (t % typesize == 0 && t != 0) {
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
	

	Bitset(const Bitset<T>& b) : Bitset(b.nBits)
	{
		for (unsigned int i = 0; i < count; i++) {
			this->data[i] = b.data[i];
		}
	}
	
	
	~Bitset()
	{
		delete[] data;
	}
	
	
	Bitset& operator&= (const Bitset& b) 
	{
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] &= b.data[i];
		}
		return *this;
	}
	
	
	Bitset& operator|= (const Bitset& b) 
	{
		for (unsigned int i = 0; i < this->count; i++) {
			this->data[i] |= b.data[i];
		}
		return *this;
	}
	
	
	Bitset operator& (const Bitset& b) 
	{
		Bitset<T> temp(*this);
		temp &= b;
		return temp;
	}
	
	
	bool operator[] (const unsigned int index) const
	{
		pair<unsigned int, T> p = getMaskByIndex(index);
		return (data[p.first] & p.second) != 0;
	}
	
	
	void set(const unsigned int index, const bool value) 
	{
		pair<unsigned int, T> p = getMaskByIndex(index);
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
	T* data;
	T lastmask;
	
	pair<unsigned int, T> getMaskByIndex(const unsigned int index) const
	{
		unsigned int typesize = sizeof(T) * 8;
		unsigned int typeindex = static_cast<unsigned int>(index / typesize);
		assert(typeindex < count);
		T mask = 1;
		mask <<= (index % typesize);
		return make_pair(typeindex, mask);
	}
};

#endif
