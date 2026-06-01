#include "DefaultAllocator.hpp"
#include <stdint.h>

namespace MathLib {
    void* DefaultAllocator::Alloc(size_t size) {
        return ::operator new(size);
    }
    bool DefaultAllocator::Dealloc(void* ptr) {
        ::operator delete(ptr);
        return true;
    }
    size_t DefaultAllocator::GetFreeMemory(void) const {
        return SIZE_MAX;
    }
    size_t DefaultAllocator::GetSize(void) const {
        return SIZE_MAX;
    }
}