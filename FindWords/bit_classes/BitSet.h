#ifndef BITSET_H
#define BITSET_H


#include <cassert>
#include <cstring>


#include "../service/LinearAllocator.h"


const uint64_t TYPESIZE_IN_BYTES = sizeof(uint64_t);
const uint64_t TYPESIZE_IN_BITS = TYPESIZE_IN_BYTES * 8;
const uint64_t ULL_MAX_VALUE = ~0ULL;


class BitSet
{
public:
    BitSet() : nBits(0), count(0), data(nullptr) {}

    BitSet(uint64_t nBits);
    BitSet(uint64_t nBits, bool** a, uint64_t n, uint64_t m);
    BitSet(const BitSet& b);

    BitSet(uint64_t nBits, LinearAllocator& linearAllocator);
    BitSet(uint64_t nBits, bool** a, uint64_t n, uint64_t m, LinearAllocator& linearAllocator);
    BitSet(const BitSet& b, LinearAllocator& linearAllocator);

    ~BitSet();

    BitSet& operator&=(const BitSet& b);
    BitSet& operator|=(const BitSet& b);
    BitSet operator&(const BitSet& b);
    BitSet operator|(const BitSet& b);
    BitSet operator~();

    bool operator[](uint64_t index);
    void set(uint64_t index, bool value);

    bool isAllTrue();
    bool isAllFalse();

private:
    uint64_t nBits;
    uint64_t count;
    uint64_t lastmask = 0;
    uint64_t* data;

    bool usingAllocator = false;


    uint64_t getTypeIndexAndMask(uint64_t index, uint64_t& mask);
    BitSet& operator=(const BitSet& b);
};

#endif
