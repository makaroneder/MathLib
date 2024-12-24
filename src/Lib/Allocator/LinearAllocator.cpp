#include "LinearAllocator.hpp"
#include "../Typedefs.hpp"

namespace MathLib {
    LinearAllocator::LinearAllocator(void* buffer, size_t size) : BufferAllocator(buffer, size), offset(0) {
        EmptyBenchmark
    }
    LinearAllocator::LinearAllocator(Allocator* allocator, size_t size) : BufferAllocator(allocator, size), offset(0) {
        EmptyBenchmark
    }
    void* LinearAllocator::Alloc(size_t size) {
        StartBenchmark
        uint8_t* ret = nullptr;
        if (offset + size < this->size) {
            ret = (uint8_t*)buffer + offset;
            offset += size;
        }
        ReturnFromBenchmark(ret);
    }
    bool LinearAllocator::Dealloc(void* ptr) {
        StartBenchmark
        ReturnFromBenchmark(IsBetween((uintptr_t)ptr, (uintptr_t)buffer, (uintptr_t)buffer + size));
    }
    size_t LinearAllocator::GetFreeMemory(void) const {
        StartBenchmark
        ReturnFromBenchmark(size - offset);
    }
}