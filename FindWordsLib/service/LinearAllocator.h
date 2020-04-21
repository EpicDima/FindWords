#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H


#include <cassert>
#include <cstdint>
#include <cstdio>

class LinearAllocator
{
public:
    explicit LinearAllocator(uint64_t memorySize) : size(memorySize)
    {
        assert(memorySize > 0);
        buffer = new uint8_t[memorySize];
    }


    ~LinearAllocator()
    {
        delete[] buffer;
    }


    void* allocate(uint64_t allocatedSize)
    {
        assert(allocatedSize > 0 && (size - offset) >= allocatedSize);
        uint8_t* allocatedPointer = buffer + offset;
        offset += allocatedSize;
        return allocatedPointer;
    }


    void deallocate(uint64_t deallocatedSize)
    {
        assert(offset >= deallocatedSize);
        offset -= deallocatedSize;
    }


    void reset()
    {
        offset = 0;
    }


private:
    uint8_t* buffer;
    uint64_t size;
    uint64_t offset = 0;
};


#endif
