#ifndef BITMASK_H
#define BITMASK_H


#include <new>
#include <stdint.h>


class BitMask
{
public:
    BitMask() : nRows(0), nCols(0), mask(nullptr) {}
    BitMask(uint64_t nRows, uint64_t nCols);
    BitMask(const BitMask& b);

    ~BitMask();

    BitMask& operator=(const BitMask& b);

    bool* operator[](const uint64_t index);

    bool** getRawMask();

private:
    uint64_t nRows, nCols;
    bool** mask;
};


#endif
