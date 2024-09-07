#ifndef MemoryManager_H
#define MemoryManager_H
#include <stddef.h>

struct MemoryManager {
    virtual void* At(size_t pos, size_t size) = 0;
    virtual size_t GetSize(void) const = 0;
};
extern MemoryManager* memoryManager;

#endif