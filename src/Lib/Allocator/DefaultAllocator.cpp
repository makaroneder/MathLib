#include "DefaultAllocator.hpp"

namespace MathLib {
    void* DefaultAllocator::Alloc(size_t size) {
        return ::operator new(size);
    }
    bool DefaultAllocator::Dealloc(void* ptr) {
        ::operator delete(ptr);
        return true;
    }
}