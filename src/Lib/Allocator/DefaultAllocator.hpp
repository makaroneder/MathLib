#ifndef MathLib_Allocator_Default_H
#define MathLib_Allocator_Default_H
#include "Allocator.hpp"

namespace MathLib {
    struct DefaultAllocator : Allocator {
        [[nodiscard]] virtual void* Alloc(size_t size) override;
        [[nodiscard]] virtual bool Dealloc(void* ptr) override;
        [[nodiscard]] virtual size_t GetFreeMemory(void) const override;
        [[nodiscard]] virtual size_t GetSize(void) const override;
    };
}

#endif