#ifndef BOOLSET_H
#define BOOLSET_H


#include <iostream>
#include <cassert>

using namespace std;


class BoolSet
{
public:
	BoolSet() {}
	
	
	BoolSet(unsigned int nBits)
	{
		this->nBits = nBits; 
		data = new bool[nBits];
		for (unsigned int i = 0; i < nBits; i++) {
			data[i] = false;
		}
	}
	
	
	BoolSet(unsigned int nBits, bool** a, unsigned int n, unsigned int m) : BoolSet(nBits)
	{
		for (unsigned int i = 0; i < n; i++) {
			for (unsigned int j = 0; j < m; j++) {
				data[i * m + j] = a[i][j];
			}
		}
	}
	

	BoolSet(const BoolSet& b) : BoolSet(b.nBits)
	{
		for (unsigned int i = 0; i < this->nBits; i++) {
			this->data[i] = b.data[i];
		}
	}
	
	
	~BoolSet()
	{
		delete[] data;
	}
	
	
	BoolSet& operator&= (const BoolSet& b) 
	{
		for (unsigned int i = 0; i < this->nBits; i++) {
			this->data[i] &= b.data[i];
		}
		return *this;
	}
	
	
	BoolSet& operator|= (const BoolSet& b) 
	{
		for (unsigned int i = 0; i < this->nBits; i++) {
			this->data[i] |= b.data[i];
		}
		return *this;
	}
	
	
	BoolSet operator& (const BoolSet& b) 
	{
		BoolSet temp(*this);
		temp &= b;
		return temp;
	}
	
	
	BoolSet operator| (const BoolSet& b)
	{
		BoolSet temp(*this);
		temp |= b;
		return temp;
	}
	
	
	BoolSet operator~ ()
	{
		BoolSet temp(*this);
		for (unsigned int i = 0; i < temp.nBits; i++) {
			temp.data[i] = !temp.data[i];
		}
		return temp;
	}
	
	
	bool operator[] (const unsigned int index)
	{
		return data[index];
	}
	
	
	void set(const unsigned int index, bool value) 
	{
		data[index] = value;
	}
	
	
    bool isAllTrue() const
    {
    	for (unsigned int i = 0; i < nBits; i++) {
			if (!data[i]) {
				return false;
			}
		}
		return true;
	}
	
	
	bool isAllFalse() const
    {
    	for (unsigned int i = 0; i < nBits; i++) {
			if (data[i]) {
				return false;
			}
		}
		return true;
	}
	
	
//private:
	unsigned int nBits;
	bool* data;

};

#endif
