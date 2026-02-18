#ifndef MathLib_Allocator_H
#define MathLib_Allocator_H
#include "Allocatable.hpp"

namespace MathLib {
    struct Allocator : Allocatable {
        virtual ~Allocator(void) override;
        [[nodiscard]] virtual void* Alloc(size_t size) = 0;
        [[nodiscard]] virtual bool Dealloc(void* ptr) = 0;
    };
    extern Allocator* allocator;
}

#endif