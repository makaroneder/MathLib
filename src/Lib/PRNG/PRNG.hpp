#ifndef MathLib_PRNG_H
#define MathLib_PRNG_H
#include "../Typedefs.hpp"

namespace MathLib {
    template <typename T>
    struct PRNG {
        PRNG(const T& seed) : seed(seed) {
            EmptyBenchmark
        }
        virtual T Random(void) = 0;
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