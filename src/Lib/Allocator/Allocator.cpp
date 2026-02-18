#include "DefaultAllocator.hpp"

namespace MathLib {
    DefaultAllocator defaultAllocator = DefaultAllocator();
    Allocator* allocator = &defaultAllocator;

    Allocator::~Allocator(void) {
        if (allocator == this) allocator = nullptr;
    }
}