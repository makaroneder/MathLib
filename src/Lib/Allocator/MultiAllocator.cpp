#include "MultiAllocator.hpp"

namespace MathLib {
    MultiAllocator::MultiAllocator(void) : allocator1(nullptr), allocator2(nullptr) {}
    MultiAllocator::MultiAllocator(Allocator* allocator) : allocator1(allocator), allocator2(nullptr) {}
    MultiAllocator::MultiAllocator(Allocator* allocator1, Allocator* allocator2) : allocator1(allocator1), allocator2(allocator2) {}
    MultiAllocator::~MultiAllocator(void) {
        if (allocator1) delete allocator1;
        if (allocator2) delete allocator2;
    }
    void* MultiAllocator::Alloc(size_t size) {
        if (allocator1) {
            void* const tmp = allocator1->Alloc(size);
            if (tmp) return tmp;
        }
        return allocator2 ? allocator2->Alloc(size) : nullptr;
    }
    bool MultiAllocator::Dealloc(void* ptr) {
        return (allocator1 && allocator1->Dealloc(ptr)) || (allocator2 && allocator2->Dealloc(ptr));
    }
    bool MultiAllocator::AddAllocator(Allocator* allocator) {
        if (!allocator) return false;
        if (!allocator1) {
            allocator1 = allocator;
            return true;
        }
        if (!allocator2) {
            allocator2 = allocator;
            return true;
        }
        MultiAllocator* const tmp = new MultiAllocator(allocator, allocator2);
        if (!tmp) return false;
        allocator2 = tmp;
        return true;
    }
}