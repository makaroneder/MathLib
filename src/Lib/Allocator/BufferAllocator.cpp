#include "BufferAllocator.hpp"
#include "../Host.hpp"

namespace MathLib {
    BufferAllocator::BufferAllocator(void* buffer, size_t size) : buffer(buffer), size(size), allocator(nullptr) {
        StartBenchmark
        if (!buffer || !size) Panic("No memory provided for allocator");
        EndBenchmark
    }
    BufferAllocator::BufferAllocator(Allocator* allocator, size_t size) : buffer(allocator && size ? allocator->Alloc(size) : nullptr), size(size), allocator(allocator) {
        StartBenchmark
        if (!allocator) Panic("No allocator provided for allocator");
        if (!size) Panic("No memory provided for allocator");
        if (!buffer) Panic("Failed to allocate memory for allocator");
        EndBenchmark
    }
    BufferAllocator::~BufferAllocator(void) {
        StartBenchmark
        if (allocator && !allocator->Dealloc(buffer)) Panic("Failed to deallocate memory");
        if (MathLib::allocator == this) MathLib::allocator = allocator;
        EndBenchmark
    }
    void* BufferAllocator::GetBuffer(void) const {
        StartBenchmark
        ReturnFromBenchmark(buffer);
    }
    size_t BufferAllocator::GetSize(void) const {
        StartBenchmark
        ReturnFromBenchmark(size);
    }
}