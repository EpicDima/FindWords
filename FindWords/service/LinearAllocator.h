#ifndef LINEAR_ALLOCATOR_H
#define LINEAR_ALLOCATOR_H


#include <cassert>
#include <stdint.h>


class LinearAllocator
{
public:
    LinearAllocator(uint64_t memorySize);
    ~LinearAllocator();

    void* allocate(uint64_t allocatedSize);
    void deallocate(uint64_t deallocatedSize);

    void reset();


private:
    uint8_t* buffer;
    uint64_t size;
    uint64_t offset = 0;
};


#endif
