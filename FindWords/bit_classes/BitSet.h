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

    BitSet(uint64_t nBits) : nBits(nBits)
    {
        count = nBits / TYPESIZE_IN_BITS;
        uint64_t difference = nBits % TYPESIZE_IN_BITS;
        if (difference != 0) {
            count++;
            lastmask = ~(~lastmask >> (TYPESIZE_IN_BITS - difference));
        }
        data = reinterpret_cast<uint64_t*>(calloc(count, TYPESIZE_IN_BYTES));
    }


    BitSet(uint64_t nBits, bool** a, uint64_t n, uint64_t m) : BitSet(nBits)
    {
        uint64_t mask = 1;
        uint64_t k = 0;
        for (uint64_t i = 0; i < n; i++) {
            for (uint64_t j = 0; j < m; j++) {
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


    BitSet(const BitSet& b) : nBits(b.nBits), count(b.count), lastmask(b.lastmask)
    {
        this->data = reinterpret_cast<uint64_t*>(malloc(this->count * TYPESIZE_IN_BYTES));

        for (uint64_t i = 0; i < this->count; i++) {
            this->data[i] = b.data[i];
        }

        // memcpy(this->data, b.data, b.count * TYPESIZE_IN_BYTES);
    }


    BitSet(uint64_t nBits, LinearAllocator& linearAllocator) : nBits(nBits), usingAllocator(true)
    {
        count = nBits / TYPESIZE_IN_BITS;
        uint64_t difference = nBits % TYPESIZE_IN_BITS;
        if (difference != 0) {
            count++;
            lastmask = ~(~lastmask >> (TYPESIZE_IN_BITS - difference));
        }
        data = reinterpret_cast<uint64_t*>(
            memset(linearAllocator.allocate(count * TYPESIZE_IN_BYTES), 0, count * TYPESIZE_IN_BYTES));
    }


    BitSet(uint64_t nBits, bool** a, uint64_t n, uint64_t m, LinearAllocator& linearAllocator)
    : BitSet(nBits, linearAllocator)
    {
        uint64_t mask = 1;
        uint64_t k = 0;
        for (uint64_t i = 0; i < n; i++) {
            for (uint64_t j = 0; j < m; j++) {
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


    BitSet(const BitSet& b, LinearAllocator& linearAllocator)
    : nBits(b.nBits), count(b.count), lastmask(b.lastmask), usingAllocator(true)
    {
        this->data = reinterpret_cast<uint64_t*>(linearAllocator.allocate(b.count * TYPESIZE_IN_BYTES));
        for (uint64_t i = 0; i < this->count; i++) {
            this->data[i] = b.data[i];
        }
    }


    ~BitSet()
    {
        if (!usingAllocator) {
            free(data);
        }
    }


    BitSet& operator&=(const BitSet& b)
    {
        assert(this->nBits == b.nBits);
        for (uint64_t i = 0; i < this->count; i++) {
            this->data[i] &= b.data[i];
        }
        return *this;
    }


    BitSet& operator|=(const BitSet& b)
    {
        assert(this->nBits == b.nBits);
        for (uint64_t i = 0; i < this->count; i++) {
            this->data[i] |= b.data[i];
        }
        return *this;
    }


    BitSet operator&(const BitSet& b)
    {
        BitSet temp(*this);
        temp &= b;
        return temp;
    }


    BitSet operator|(const BitSet& b)
    {
        BitSet temp(*this);
        temp |= b;
        return temp;
    }


    BitSet operator~()
    {
        BitSet temp(*this);
        for (uint64_t i = 0; i < temp.count; i++) {
            temp.data[i] = ~temp.data[i];
        }
        temp.data[temp.count - 1] = ~temp.lastmask & temp.data[temp.count - 1];
        return temp;
    }


    bool operator[](uint64_t index)
    {
        uint64_t mask;
        return (data[getTypeIndexAndMask(index, mask)] & mask) != 0;
    }


    void set(uint64_t index, bool value)
    {
        uint64_t mask;
        if (value) {
            data[getTypeIndexAndMask(index, mask)] |= mask;
        } else {
            data[getTypeIndexAndMask(index, mask)] &= ~mask;
        }
    }


    bool isAllTrue()
    {
        for (uint64_t i = 0; i < count - 1; i++) {
            if (data[i] != ULL_MAX_VALUE) {
                return false;
            }
        }
        return (data[count - 1] | lastmask) == ULL_MAX_VALUE;
    }


    bool isAllFalse()
    {
        for (uint64_t i = 0; i < count; i++) {
            if (data[i] != 0) {
                return false;
            }
        }
        return true;
    }

private:
    uint64_t nBits;
    uint64_t count;
    uint64_t lastmask = 0;
    uint64_t* data;

    bool usingAllocator = false;


    uint64_t getTypeIndexAndMask(uint64_t index, uint64_t& mask)
    {
        assert(index < nBits);
        mask = 1ULL << (index % TYPESIZE_IN_BITS);
        return index / TYPESIZE_IN_BITS;
    }


    BitSet& operator=(const BitSet& b)
    {
        return *this;
    }
};

#endif
