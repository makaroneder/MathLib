#ifndef MathLib_Allocator_Allocatable_H
#define MathLib_Allocator_Allocatable_H
#include <stddef.h>

namespace MathLib {
    struct Allocatable {
        /// @brief Virtual destructor
        virtual ~Allocatable(void);
        [[nodiscard]] static void* operator new(size_t size);
        [[nodiscard]] static void* operator new[](size_t size);
        static void operator delete(void* ptr);
        static void operator delete[](void* ptr);
    };
}

#endif