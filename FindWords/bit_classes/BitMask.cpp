#include "BitMask.h"


BitMask::BitMask(uint64_t nRows, uint64_t nCols) : nRows(nRows), nCols(nCols)
{
    mask = new bool*[nRows];
    for (uint64_t i = 0; i < nRows; i++) {
        mask[i] = new bool[nCols];
    }
}


BitMask::BitMask(const BitMask& b) : BitMask(b.nRows, b.nCols)
{
    for (uint64_t i = 0; i < nRows; i++) {
        for (uint64_t j = 0; j < nCols; j++) {
            this->mask[i][j] = b.mask[i][j];
        }
    }
}


BitMask::~BitMask()
{
    // for (uint64_t i = 0; i < nRows; i++) {
    //     delete[] mask[i];
    // }
    // delete[] mask;
}


BitMask& BitMask::operator=(const BitMask& b)
{
    if (this != &b) {
        this->~BitMask();
        new (this) BitMask(b);
    }
    return *this;
}


bool* BitMask::operator[](const uint64_t index)
{
    return mask[index];
}


bool** BitMask::getRawMask()
{
    return mask;
}