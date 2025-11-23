#include "BufferAllocator.hpp"
#include "../Host.hpp"

namespace MathLib {
    BufferAllocator::BufferAllocator(void* buffer, size_t size) : buffer(buffer), size(size), allocator(nullptr) {
        if (!buffer || !size) Panic("No memory provided for allocator");
    }
    BufferAllocator::BufferAllocator(Allocator* allocator, size_t size) : buffer(allocator && size ? allocator->Alloc(size) : nullptr), size(size), allocator(allocator) {
        if (!allocator) Panic("No allocator provided for allocator");
        if (!size) Panic("No memory provided for allocator");
        if (!buffer) Panic("Failed to allocate memory for allocator");
    }
    BufferAllocator::~BufferAllocator(void) {
        if (allocator && !allocator->Dealloc(buffer)) Panic("Failed to deallocate memory");
        if (MathLib::allocator == this) MathLib::allocator = allocator;
    }
    bool BufferAllocator::Save(Writable& file) const {
        return file.WriteBuffer(buffer, size);
    }
    bool BufferAllocator::Load(Readable& file) {
        return file.ReadBuffer(buffer, size);
    }
    size_t BufferAllocator::GetSize(void) const {
        return size;
    }
    size_t BufferAllocator::ReadPositionedSizedBuffer(void* buff, size_t s, size_t position) {
        size_t rsize = s;
        if (rsize + position >= size) rsize = size - position;
        MemoryCopy((const uint8_t*)buffer + position, buff, rsize);
        return rsize;
    }
    size_t BufferAllocator::WritePositionedSizedBuffer(const void* buff, size_t s, size_t position) {
        size_t rsize = s;
        if (rsize + position >= size) rsize = size - position;
        MemoryCopy(buff, (uint8_t*)buffer + position, rsize);
        return rsize;
    }
    void* BufferAllocator::GetBuffer(void) const {
        return buffer;
    }
}