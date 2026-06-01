#include "Memory.hpp"
#include <Host.hpp>
#include <Pair.hpp>
#include <Memory.hpp>
#include <Logger.hpp>
#include <Allocator/RegionAllocator.hpp>

uint8_t memoryRegion[15 * 4096];
MathLib::RegionAllocator initialAllocator = MathLib::RegionAllocator(memoryRegion, SizeOfArray(memoryRegion));
MathLib::MultiAllocator allocator = MathLib::MultiAllocator(&initialAllocator);
size_t memorySize = 0;
MathLib::Pair<char, size_t> sizes[] = {
    MathLib::Pair<char, size_t>('\0', 0), MathLib::Pair<char, size_t>('K', 0),
    MathLib::Pair<char, size_t>('M', 0), MathLib::Pair<char, size_t>('G', 0),
};
const size_t sizeOfSizes = SizeOfArray(sizes);
extern uintptr_t kernelStart;
extern uintptr_t kernelEnd;

bool AddMemoryRegion(uintptr_t address, size_t size) {
    if (!address && size > 4096) {
        address += 4096;
        size -= 4096;
    }
    if (!(address && size)) return true;
    const uintptr_t end = address + size;
    if (IsBetween(address, (uintptr_t)&kernelStart, (uintptr_t)&kernelEnd - 1))
        return end <= (uintptr_t)&kernelEnd || allocator.AddAllocator(new MathLib::RegionAllocator((void*)&kernelEnd, end - (uintptr_t)&kernelEnd));
    if (IsBetween(end, (uintptr_t)&kernelStart, (uintptr_t)&kernelEnd))
        return (uintptr_t)&kernelStart <= address || allocator.AddAllocator(new MathLib::RegionAllocator((void*)address, (uintptr_t)&kernelStart - address));
    if (IsBetween((uintptr_t)&kernelStart, address, end - 1)) {
        if (!((uintptr_t)&kernelStart <= address || allocator.AddAllocator(new MathLib::RegionAllocator((void*)address, (uintptr_t)&kernelStart - address)))) return false;
        return end <= (uintptr_t)&kernelEnd || allocator.AddAllocator(new MathLib::RegionAllocator((void*)&kernelEnd, end - (uintptr_t)&kernelEnd));
    }
    if (IsBetween((uintptr_t)&kernelEnd, address, end)) return end == (uintptr_t)&kernelEnd || allocator.AddAllocator(new MathLib::RegionAllocator((void*)&kernelEnd, end - (uintptr_t)&kernelEnd));
    return allocator.AddAllocator(new MathLib::RegionAllocator((void*)address, size));
}
MathLib::String PrintSize(size_t value) {
    size_t tmp = value;
    for (size_t i = 0; i < sizeOfSizes; i++) {
        if (i + 1 == sizeOfSizes) {
            sizes[i].second = tmp;
            continue;
        }
        sizes[i].second = tmp % 1024;
        tmp /= 1024;
    }
    MathLib::String ret;
    bool first = true;
    for (size_t i = sizeOfSizes; i; i--) {
        if (!(sizes[i - 1].second || (first && i == 1))) continue;
        if (!first) ret += " + ";
        ret += MathLib::ToString(sizes[i - 1].second, 10);
        ret += sizes[i - 1].first;
        ret += 'B';
        first = false;
    }
    return ret;
}
MathLib::String ReportFreeMemory(void) {
    return "Free memory: "_M + PrintSize(allocator.GetFreeMemory()) + "\nUsable memory: " + PrintSize(allocator.GetSize()) + "\nTotal memory: " + PrintSize(memorySize) + '\n';
}
void* operator new(size_t size) {
    return allocator.Alloc(size);
}
void* operator new[](size_t size) {
    return allocator.Alloc(size);
}
void* operator new(size_t size, std::align_val_t align) {
    uintptr_t tmp = (uintptr_t)allocator.Alloc(size + (size_t)align - 1);
    while (tmp % (size_t)align) tmp++;
    return (void*)tmp;
}
void* operator new[](size_t size, std::align_val_t align) {
    uintptr_t tmp = (uintptr_t)allocator.Alloc(size + (size_t)align - 1);
    while (tmp % (size_t)align) tmp++;
    return (void*)tmp;
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