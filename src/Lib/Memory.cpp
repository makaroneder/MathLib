#include "Memory.hpp"
#include <stdint.h>

namespace MathLib {
    void MemoryCopy(const void* src, void* dst, size_t size) {
        #ifdef __x86_64__
        asm volatile("rep movsb" : "+D"(dst), "+S"(src), "+c"(size) :: "memory", "cc");
        #else
        const uint8_t* src8 = (const uint8_t*)src;
        uint8_t* dst8 = (uint8_t*)dst;
        for (size_t i = 0; i < size; i++) dst8[i] = src8[i];
        #endif
    }
}