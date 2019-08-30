#ifndef BITMASK_H
#define BITMASK_H


class BitMask
{
public:
    BitMask() : nRows(0), nCols(0), mask(nullptr) {}


    BitMask(unsigned int nRows, unsigned int nCols) : nRows(nRows), nCols(nCols)
    {
        mask = new bool*[nRows];
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
        // Если раскомментировать появляетсся ошибка с кучей (файл ntdll.dll)
        // Нужно как-то всё тщательно проанализировать, а пока будет утечка памяти
        /*
        for (unsigned int i = 0; i < nRows; i++) {
            delete[] mask[i];
        }
        delete[] mask;
        */
    }


	BitMask& operator=(const BitMask& b)
	{
		if (this != &b) {
			this->~BitMask();
			new (this) BitMask(b);
		}
		return *this;
	}


    bool* operator[](const unsigned int index)
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
