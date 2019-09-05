#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H


#include <cassert>

typedef unsigned char byte;
typedef unsigned long long ull_t;


class LinearAllocator
{
public:
    LinearAllocator(ull_t memorySize) : size(memorySize)
    {
        assert(memorySize > 0);
        buffer = new byte[memorySize];
    }


    ~LinearAllocator()
    {
        delete[] buffer;
    }


    void* allocate(ull_t allocatedSize)
    {
        assert(allocatedSize > 0 && (size - offset) >= allocatedSize);
        byte* allocatedPointer = buffer + offset;
        offset += allocatedSize;
        return allocatedPointer;
    }


    void deallocate(ull_t deallocatedSize)
    {
        assert(offset >= deallocatedSize);
        offset -= deallocatedSize;
    }


    void reset()
    {
        offset = 0;
    }


private:
    byte* buffer;
    ull_t size;
    ull_t offset = 0;
};


#endif
