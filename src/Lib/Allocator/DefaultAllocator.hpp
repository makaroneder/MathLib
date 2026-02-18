#ifndef MathLib_Allocator_Default_H
#define MathLib_Allocator_Default_H
#include "Allocator.hpp"

namespace MathLib {
    struct DefaultAllocator : Allocator {
        [[nodiscard]] virtual void* Alloc(size_t size) override;
        [[nodiscard]] virtual bool Dealloc(void* ptr) override;
    };
}

#endif