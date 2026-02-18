#ifndef MathLib_Memory_H
#define MathLib_Memory_H
#include <stddef.h>
#include <stdint.h>

namespace MathLib {
    void MemoryCopy(const void* src, void* dst, size_t size);
    template <typename T>
    #ifdef __x86_64__
    void MemorySet(void* dst, size_t size, T value);
    #else
    void MemorySet(void* dst, size_t size, T value) {
        T* dst8 = (T*)dst;
        for (size_t i = 0; i < size; i++) dst8[i] = value;
    }
    #endif
}

#endif