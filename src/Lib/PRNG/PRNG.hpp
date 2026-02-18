#ifndef MathLib_PRNG_H
#define MathLib_PRNG_H
#include "../Allocator/Allocatable.hpp"

namespace MathLib {
    template <typename T>
    struct PRNG : Allocatable {
        PRNG(const T& seed) : seed(seed) {}
        [[nodiscard]] virtual T Random(void) = 0;
        void SetSeed(const T& s) {
            seed = s;
        }
    
        protected:
        T seed;
    };
}

#endif