#include "XorSwap.hpp"
#include "Benchmark.hpp"

namespace MathLib {
    void XorSwap(uint64_t& x, uint64_t& y) {
        StartBenchmark
        x ^= y;
        y ^= x;
        x ^= y;
        EndBenchmark
    }
}