#ifndef MathLib_PRNG_H
#define MathLib_PRNG_H
#include "../Allocator/Allocatable.hpp"
#include "../Benchmark.hpp"

namespace MathLib {
    template <typename T>
    struct PRNG : Allocatable {
        PRNG(const T& seed) : seed(seed) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual T Random(void) = 0;
        void SetSeed(const T& s) {
            StartBenchmark
            seed = s;
            EndBenchmark
        }
    
        protected:
        T seed;
    };
}

#endif