#ifndef MemoryWatcher_H
#define MemoryWatcher_H
#include <stddef.h>

struct MemoryWatcher {
    MemoryWatcher(void);
    /// @brief Virtual destructor
    virtual ~MemoryWatcher(void);
    virtual void OnAlloc(void* addr, size_t size) = 0;
    virtual void OnDealloc(void* addr) = 0;
};
extern MemoryWatcher* memoryWatcher;

#endif