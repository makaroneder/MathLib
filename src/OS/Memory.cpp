#include "MemoryBuffer.hpp"
#include "Memory.hpp"
#include <Host.hpp>

MemoryBuffer<1000 * 4096> tmpMemoryManager;
MemoryManager* memoryManager = &tmpMemoryManager;
struct AllocationHeader {
    static constexpr uint8_t expectedSignature = '!';
    uint8_t signature;
    bool free;
    size_t size;

    AllocationHeader(size_t size);
    bool IsValid(void) const {
        return signature == expectedSignature;
    }
} __attribute__((packed));
AllocationHeader::AllocationHeader(size_t size) : signature(expectedSignature), free(false), size(size) {}

void* Alloc(size_t size) {
    if (!memoryManager) return nullptr;
    size_t pos = 0;
    const size_t memorySize = memoryManager->GetSize();
    while (pos < memorySize) {
        AllocationHeader* header = (AllocationHeader*)memoryManager->At(pos, sizeof(AllocationHeader) + size);
        pos += sizeof(AllocationHeader);
        uint8_t* buff = (uint8_t*)((uintptr_t)header + sizeof(AllocationHeader));
        if (!header->IsValid()) {
            *header = AllocationHeader(size);
            for (size_t i = 0; i < size; i++) buff[i] = 0;
            return buff;
        }
        else if (header->free && header->size >= size) {
            header->free = false;
            for (size_t i = 0; i < header->size; i++) buff[i] = 0;
            return buff;
        }
        pos += header->size;
    }
    return nullptr;
}
void Dealloc(void* ptr) {
    AllocationHeader* header = (AllocationHeader*)((uintptr_t)ptr - sizeof(AllocationHeader));
    if (!header->IsValid()) MathLib::Panic("Allocation header is not valid");
    if (header->free) MathLib::Panic("Double deallocation detected");
    header->free = true;
}
void* operator new(size_t size) {
    return Alloc(size);
}
void* operator new[](size_t size) {
    return Alloc(size);
}
void operator delete(void* ptr) {
    return Dealloc(ptr);
}
void operator delete[](void* ptr) {
    return Dealloc(ptr);
}
void operator delete(void* ptr, size_t size) {
    (void)size;
    return Dealloc(ptr);
}
void operator delete[](void* ptr, size_t size) {
    (void)size;
    return Dealloc(ptr);
}
void* memcpy(void* dst, const void* src, size_t n) {
    uint8_t* dst8 = (uint8_t*)dst;
    const uint8_t* src8 = (const uint8_t*)src;
    for (size_t i = 0; i < n; i++) dst8[i] = src8[i];
    return dst;
}
void* memset(void *s, int c, size_t n) {
    uint8_t* p = (uint8_t*)s;
    for (size_t i = 0; i < n; i++) p[i] = (uint8_t)c;
    return s;
}
void* memmove(void* dst, const void* src, size_t n) {
    uint8_t* dst8 = (uint8_t*)dst;
    const uint8_t* src8 = (const uint8_t*)src;
    if (src > dst)
        for (size_t i = 0; i < n; i++) dst8[i] = src8[i];
    else if (src < dst)
        for (size_t i = n; i > 0; i--) dst8[i - 1] = src8[i - 1];
    return dst;
}
int memcmp(const void* s1, const void* s2, size_t n) {
    const uint8_t* p1 = (const uint8_t*)s1;
    const uint8_t* p2 = (const uint8_t*)s2;
    for (size_t i = 0; i < n; i++)
        if (p1[i] != p2[i]) return p1[i] < p2[i] ? -1 : 1;
    return 0;
}