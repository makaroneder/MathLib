#ifndef MathLib_Allocator_Linear_H
#define MathLib_Allocator_Linear_H
#include "BufferAllocator.hpp"

namespace MathLib {
    struct LinearAllocator : BufferAllocator {
        LinearAllocator(void* buffer, size_t size);
        LinearAllocator(Allocator* allocator, size_t size);
        virtual void* Alloc(size_t size) override;
        virtual bool Dealloc(void* ptr) override;
        virtual size_t GetFreeMemory(void) const override;

        private:
        size_t offset;
    };
}

#endif