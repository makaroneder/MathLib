#ifndef MemoryBuffer_H
#define MemoryBuffer_H
#include "MemoryManager.hpp"
#include <stdint.h>

template <size_t size>
struct MemoryBuffer : MemoryManager {
    virtual void* At(size_t pos, size_t size_) override {
        return pos + size_ < size ? &buffer[pos] : nullptr;
    }
    virtual size_t GetSize(void) const override {
        return size;
    }

    private:
    uint8_t buffer[size];
};

#endif