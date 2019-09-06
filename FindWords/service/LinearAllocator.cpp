#include "LinearAllocator.h"

LinearAllocator::LinearAllocator(uint64_t memorySize) : size(memorySize)
{
    assert(memorySize > 0);
    buffer = new uint8_t[memorySize];
}


LinearAllocator::~LinearAllocator()
{
    delete[] buffer;
}


void* LinearAllocator::allocate(uint64_t allocatedSize)
{
    assert(allocatedSize > 0 && (size - offset) >= allocatedSize);
    uint8_t* allocatedPointer = buffer + offset;
    offset += allocatedSize;
    return allocatedPointer;
}


void LinearAllocator::deallocate(uint64_t deallocatedSize)
{
    assert(offset >= deallocatedSize);
    offset -= deallocatedSize;
}


void LinearAllocator::reset()
{
    offset = 0;
}