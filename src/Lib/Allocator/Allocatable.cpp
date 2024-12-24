#include "Allocator.hpp"
#include "../Host.hpp"

namespace MathLib {
    Allocatable::~Allocatable(void) {
        EmptyBenchmark
    }
    void* Allocatable::operator new(size_t size) {
        StartBenchmark
        ReturnFromBenchmark(allocator ? allocator->Alloc(size) : nullptr);
    }
    void* Allocatable::operator new[](size_t size) {
        StartBenchmark
        ReturnFromBenchmark(allocator ? allocator->Alloc(size) : nullptr);
    }
    void Allocatable::operator delete(void* ptr) {
        StartBenchmark
        if (!allocator || !allocator->Dealloc(ptr)) Panic("Failed to deallocate memory");
        EndBenchmark
    }
    void Allocatable::operator delete[](void* ptr) {
        StartBenchmark
        if (!allocator || !allocator->Dealloc(ptr)) Panic("Failed to deallocate memory");
        EndBenchmark
    }
}