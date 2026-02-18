#include "Allocator.hpp"

namespace MathLib {
    [[noreturn]] void Panic(const char* str);
    Allocatable::~Allocatable(void) {}
    void* Allocatable::operator new(size_t size) {
        return allocator ? allocator->Alloc(size) : nullptr;
    }
    void* Allocatable::operator new[](size_t size) {
        return allocator ? allocator->Alloc(size) : nullptr;
    }
    void Allocatable::operator delete(void* ptr) {
        if (!allocator || !allocator->Dealloc(ptr)) Panic("Failed to deallocate memory");
    }
    void Allocatable::operator delete[](void* ptr) {
        if (!allocator || !allocator->Dealloc(ptr)) Panic("Failed to deallocate memory");
    }
}