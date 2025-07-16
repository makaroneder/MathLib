#include "DefaultAllocator.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    void* DefaultAllocator::Alloc(size_t size) {
        StartAndReturnFromBenchmark(::operator new(size));
    }
    bool DefaultAllocator::Dealloc(void* ptr) {
        StartBenchmark
        ::operator delete(ptr);
        ReturnFromBenchmark(true);
    }
}