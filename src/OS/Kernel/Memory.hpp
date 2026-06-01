#ifndef Memory_H
#define Memory_H
#include <Allocator/MultiAllocator.hpp>
#include <Typedefs.hpp>

extern MathLib::MultiAllocator allocator;
extern size_t memorySize;

[[nodiscard]] bool AddMemoryRegion(uintptr_t address, size_t size);
[[nodiscard]] MathLib::String ReportFreeMemory(void);
[[nodiscard]] void* operator new(size_t size);
[[nodiscard]] void* operator new[](size_t size);
void operator delete(void* ptr);
void operator delete[](void* ptr);
void operator delete(void* ptr, size_t size);
void operator delete[](void* ptr, size_t size);
extern "C" [[nodiscard]] void* memcpy(void* dst, const void* src, size_t size);
extern "C" [[nodiscard]] void* memset(void* dst, int value, size_t size);
extern "C" [[nodiscard]] void* memmove(void* dst, const void* src, size_t size);
extern "C" [[nodiscard]] int memcmp(const void* a, const void* b, size_t size);

#endif