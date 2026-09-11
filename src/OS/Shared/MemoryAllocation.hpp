#ifndef Shared_MemoryAllocation_H
#define Shared_MemoryAllocation_H
#include <Allocator/Allocator.hpp>

extern MathLib::Allocator* globalAllocator;
[[nodiscard]] void* operator new(size_t size);
[[nodiscard]] void* operator new[](size_t size);
void operator delete(void* ptr);
void operator delete[](void* ptr);
void operator delete(void* ptr, size_t size);
void operator delete[](void* ptr, size_t size);

#endif