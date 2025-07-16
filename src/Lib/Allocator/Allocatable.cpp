#include "Allocator.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    Allocatable::~Allocatable(void) {
        EmptyBenchmark
    }
    void* Allocatable::operator new(size_t size) {
        StartAndReturnFromBenchmark(allocator ? allocator->Alloc(size) : nullptr);
    }
    void* Allocatable::operator new[](size_t size) {
        StartAndReturnFromBenchmark(allocator ? allocator->Alloc(size) : nullptr);
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