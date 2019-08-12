#ifndef BITMASK_H
#define BITMASK_H


class BitMask
{
public:
	BitMask(unsigned int nRows, unsigned int nCols)
	{
		this->nRows = nRows;
		this->nCols = nCols;
		
		mask = new bool* [nRows];
		for (unsigned int i = 0; i < nRows; i++) {
			mask[i] = new bool[nCols];
		}
	}
	
	
	BitMask(const BitMask& b) : BitMask(b.nRows, b.nCols)
	{
		for (unsigned int i = 0; i < nRows; i++) {
			for (unsigned int j = 0; j < nCols; j++) {
				this->mask[i][j] = b.mask[i][j];
			}
		}
	}
	
	
	~BitMask()
	{
		/*
		for (unsigned int i = 0; i < nRows; i++) {
			delete[] mask[i];
		}
		delete[] mask;
		*/
	}
	
	
	bool* operator[] (const unsigned int index)
	{
		return mask[index];
	}
	
	
	bool** getRawMask()
	{
		return mask;
	}
	
	
private:
	unsigned int nRows, nCols;
	bool** mask;
};

#endif
