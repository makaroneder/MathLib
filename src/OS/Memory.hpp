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
void* memcpy(void* dst, const void* src, size_t n);
void* memset(void *s, int c, size_t n);
void* memmove(void* dst, const void* src, size_t n);
int memcmp(const void* s1, const void* s2, size_t n);

#endif