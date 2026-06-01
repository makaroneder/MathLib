#ifndef MathLib_Memory_H
#define MathLib_Memory_H
#include <stddef.h>
#include <stdint.h>

namespace MathLib {
    void MemoryCopy(const void* src, void* dst, size_t size);
    void MemorySet8(void* dst, size_t size, uint8_t value);
    void MemorySet16(void* dst, size_t size, uint16_t value);
    void MemorySet32(void* dst, size_t size, uint32_t value);
    void MemorySet64(void* dst, size_t size, uint64_t value);
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