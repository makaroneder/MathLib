#ifndef Memory_H
#define Memory_H
#include <stddef.h>
#include <stdint.h>

struct MemoryManager {
    virtual void* At(size_t pos) = 0;
    virtual size_t GetSize(void) const = 0;
};
template <size_t size>
struct MemoryBuffer : MemoryManager {
    virtual void* At(size_t pos) override {
        return pos < size ? &buffer[pos] : nullptr;
    }
    virtual size_t GetSize(void) const override {
        return size;
    }

    private:
    uint8_t buffer[size];
};

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