#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H


#include <vector>


typedef unsigned char byte;
typedef unsigned long long ull_t;


class StackAllocator
{
public:
    StackAllocator(ull_t memorySize) : size(memorySize)
    {
        buffer = new byte[memorySize];
        previousOffsets.reserve(memorySize / 128 > 128 ? memorySize / 128 : 128);
    }


    ~StackAllocator()
    {
        delete[] buffer;
        previousOffsets.clear();
    }


    void* allocate(ull_t allocatedSize)
    {
        if (allocatedSize == 0 || (size - offset) < allocatedSize) {
            return nullptr;
        }

        byte* allocatedPointer = buffer + offset;
        previousOffsets.push_back(offset);
        previousOffsetsLength++;
        offset += allocatedSize;
        return allocatedPointer;
    }


    void deallocate(ull_t numDeallocations)
    {
        for (ull_t i = 1; i < numDeallocations; i++) {
            previousOffsets.pop_back();
            previousOffsetsLength--;
        }
        offset = previousOffsets[previousOffsetsLength--];
        previousOffsets.pop_back();
    }


    void deallocate()
    {
        offset = previousOffsets[previousOffsetsLength--];
        previousOffsets.pop_back();
    }


    void reset()
    {
        offset = 0;
        previousOffsetsLength = 0;
        previousOffsets.clear();
    }


private:
    byte* buffer;
    ull_t size;
    ull_t offset = 0;
    ull_t previousOffsetsLength = 0;
    std::vector<ull_t> previousOffsets;
};


#endif
