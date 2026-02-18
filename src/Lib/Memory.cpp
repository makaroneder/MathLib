#include "Memory.hpp"

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
    #ifdef __x86_64__
    template <>
    void MemorySet(void* dst, size_t size, uint8_t value) {
        asm volatile("rep stosb" : "+D"(dst), "+c"(size), "+a"(value) :: "memory", "cc");
    }
    template <>
    void MemorySet(void* dst, size_t size, uint16_t value) {
        asm volatile("rep stosw" : "+D"(dst), "+c"(size), "+a"(value) :: "memory", "cc");
    }
    template <>
    void MemorySet(void* dst, size_t size, uint32_t value) {
        asm volatile("rep stosl" : "+D"(dst), "+c"(size), "+a"(value) :: "memory", "cc");
    }
    template <>
    void MemorySet(void* dst, size_t size, uint64_t value) {
        asm volatile("rep stosq" : "+D"(dst), "+c"(size), "+a"(value) :: "memory", "cc");
    }
    #endif
}