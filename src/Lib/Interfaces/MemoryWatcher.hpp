#ifndef Interfaces_MemoryWatcher_H
#define Interfaces_MemoryWatcher_H
#include <stddef.h>

namespace MathLib {
    struct MemoryWatcher {
        MemoryWatcher(void);
        /// @brief Virtual destructor
        virtual ~MemoryWatcher(void);
        virtual void OnAlloc(void* addr, size_t size) = 0;
        virtual void OnDealloc(void* addr) = 0;
    };
    extern MemoryWatcher* memoryWatcher;
}

#endif