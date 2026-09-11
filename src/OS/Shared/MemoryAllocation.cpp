#include "MemoryAllocation.hpp"
#include <Typedefs.hpp>

void* operator new(size_t size) {
    return globalAllocator ? globalAllocator->Alloc(size) : globalAllocator;
}
void* operator new[](size_t size) {
    return globalAllocator ? globalAllocator->Alloc(size) : globalAllocator;
}
void* operator new(size_t size, std::align_val_t align) {
    if (!globalAllocator) return globalAllocator;
    uintptr_t tmp = (uintptr_t)globalAllocator->Alloc(size + (size_t)align - 1);
    while (tmp % (size_t)align) tmp++;
    return (void*)tmp;
}
void* operator new[](size_t size, std::align_val_t align) {
    if (!globalAllocator) return globalAllocator;
    uintptr_t tmp = (uintptr_t)globalAllocator->Alloc(size + (size_t)align - 1);
    while (tmp % (size_t)align) tmp++;
    return (void*)tmp;
}
void operator delete(void* ptr) {
    if (!globalAllocator || !globalAllocator->Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}
void operator delete[](void* ptr) {
    if (!globalAllocator || !globalAllocator->Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}
void operator delete(void* ptr, size_t) {
    if (!globalAllocator || !globalAllocator->Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}
void operator delete[](void* ptr, size_t) {
    if (!globalAllocator || !globalAllocator->Dealloc(ptr)) MathLib::Panic("Failed to deallocate memory");
}