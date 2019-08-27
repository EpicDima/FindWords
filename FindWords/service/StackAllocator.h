#ifndef STACK_ALLOCATOR_H
#define STACK_ALLOCATOR_H


#include <stack>


typedef unsigned char byte;
typedef unsigned long long ull_t;


class StackAllocator
{
public:
    StackAllocator(ull_t memorySize) : size(memorySize)
    {
        buffer = new byte[size];
    }


    ~StackAllocator()
    {
        delete [] buffer;
    }


    void* allocate(ull_t allocatedSize)
    {
        if (allocatedSize == 0 || (size - offset) < allocatedSize) {
            return nullptr;
        }

        byte* allocatedPointer = buffer + offset;
        previousOffsets.push(offset);
        offset += allocatedSize;
        return allocatedPointer;
    }


    void deallocate()
    {
        if (!previousOffsets.empty()) {
            offset = previousOffsets.top();
            previousOffsets.pop();
        }
    }


    void reset()
    {
        offset = 0;
        while (!previousOffsets.empty()) {
            previousOffsets.pop();
        }
    }


private:
    byte* buffer;
    ull_t size;
    ull_t offset = 0;
    std::stack<ull_t> previousOffsets;

};


#endif
