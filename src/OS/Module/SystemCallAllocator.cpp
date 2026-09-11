#include "SystemCallAllocator.hpp"
#include "SystemCall.hpp"

SystemCallAllocator allocator;
MathLib::Allocator* globalAllocator = &allocator;
void* SystemCallAllocator::Alloc(size_t size) {
    return (void*)PerformSystemCall<uint64_t>(SystemCall::AllocateMemory, size);
}
bool SystemCallAllocator::Dealloc(void* ptr) {
    (void)PerformSystemCall<uint64_t>(SystemCall::FreeMemory, (uintptr_t)ptr);
    return true;
}
size_t SystemCallAllocator::GetFreeMemory(void) const {
    // TODO:
    return 0;
}
size_t SystemCallAllocator::GetSize(void) const {
    // TODO:
    return 0;
}