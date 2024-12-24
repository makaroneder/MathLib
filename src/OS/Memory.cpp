#include "MemoryBuffer.hpp"
#include "Memory.hpp"
#include <Host.hpp>
#include <Memory.hpp>

// TODO: Use Allocator from MathLib

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