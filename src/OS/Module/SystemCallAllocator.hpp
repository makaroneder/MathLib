#ifndef Module_SystemCallAllocator_H
#define Module_SystemCallAllocator_H
#include <Allocator/Allocator.hpp>

struct SystemCallAllocator : MathLib::Allocator {
    [[nodiscard]] virtual void* Alloc(size_t size) override;
    [[nodiscard]] virtual bool Dealloc(void* ptr) override;
    [[nodiscard]] virtual size_t GetFreeMemory(void) const override;
    [[nodiscard]] virtual size_t GetSize(void) const override;
};

#endif