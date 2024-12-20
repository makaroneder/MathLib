#ifndef Interfaces_Allocatable_H
#define Interfaces_Allocatable_H
#include "MemoryWatcher.hpp"

namespace MathLib {
    struct Allocatable {
        /// @brief Virtual destructor
        virtual ~Allocatable(void);
        static void* operator new(size_t size);
        static void* operator new[](size_t size);
        static void operator delete(void* ptr);
        static void operator delete[](void* ptr);
    };
}

#endif