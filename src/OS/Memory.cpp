#include "Memory.hpp"
#include <Host.hpp>
#include <Memory.hpp>
#include <Allocator/RegionAllocator.hpp>

uint8_t memoryRegion[15 * 4096];
MathLib::RegionAllocator initialAllocator = MathLib::RegionAllocator(memoryRegion, SizeOfArray(memoryRegion));
MathLib::MultiAllocator allocator = MathLib::MultiAllocator(&initialAllocator);

void* operator new(size_t size) {
    return allocator.Alloc(size);
}
void* operator new[](size_t size) {
    return allocator.Alloc(size);
}
void* operator new(size_t size, std::align_val_t align) {
    const uintptr_t tmp = (uintptr_t)allocator.Alloc(size + (size_t)align - 1);
    return (void*)(tmp + tmp % (size_t)align);
}
void* operator new[](size_t size, std::align_val_t align) {
    const uintptr_t tmp = (uintptr_t)allocator.Alloc(size + (size_t)align - 1);
    return (void*)(tmp + tmp % (size_t)align);
}
void operator delete(void* ptr) {
    if (!allocator.Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}
void operator delete[](void* ptr) {
    if (!allocator.Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}
void operator delete(void* ptr, size_t) {
    if (!allocator.Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}
void operator delete[](void* ptr, size_t) {
    if (!allocator.Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}
void* memcpy(void* dst, const void* src, size_t size) {
    MathLib::MemoryCopy(src, dst, size);
    return dst;
}
void* memset(void* dst, int value, size_t size) {
    uint8_t* dst8 = (uint8_t*)dst;
    for (size_t i = 0; i < size; i++) dst8[i] = (uint8_t)value;
    return dst;
}
void* memmove(void* dst, const void* src, size_t size) {
    uint8_t* dst8 = (uint8_t*)dst;
    const uint8_t* src8 = (const uint8_t*)src;
    if (src > dst)
        for (size_t i = 0; i < size; i++) dst8[i] = src8[i];
    else if (src < dst)
        for (size_t i = size; i > 0; i--) dst8[i - 1] = src8[i - 1];
    return dst;
}
int memcmp(const void* a, const void* b, size_t size) {
    const uint8_t* a8 = (const uint8_t*)a;
    const uint8_t* b8 = (const uint8_t*)b;
    for (size_t i = 0; i < size; i++)
        if (a8[i] != b8[i]) return a8[i] < b8[i] ? -1 : 1;
    return 0;
}