#ifndef MathLib_Allocator_H
#define MathLib_Allocator_H
#include "Allocatable.hpp"

namespace MathLib {
    struct Allocator : Allocatable {
        virtual void* Alloc(size_t size) = 0;
        virtual bool Dealloc(void* ptr) = 0;
    };
    extern Allocator* allocator;
}

#endif