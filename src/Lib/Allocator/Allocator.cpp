#include "DefaultAllocator.hpp"

namespace MathLib {
    DefaultAllocator defaultAllocator = DefaultAllocator();
    Allocator* allocator = &defaultAllocator;
}