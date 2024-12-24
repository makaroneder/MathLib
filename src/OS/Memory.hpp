#ifndef Memory_H
#define Memory_H
#include "MemoryManager.hpp"

void* Alloc(size_t size);
void Dealloc(void* ptr);
void* operator new(size_t size);
void* operator new[](size_t size);
void operator delete(void* ptr);
void operator delete[](void* ptr);
void operator delete(void* ptr, size_t size);
void operator delete[](void* ptr, size_t size);
void* memcpy(void* dst, const void* src, size_t size);
void* memset(void* dst, int value, size_t size);
void* memmove(void* dst, const void* src, size_t size);
int memcmp(const void* a, const void* b, size_t size);

#endif