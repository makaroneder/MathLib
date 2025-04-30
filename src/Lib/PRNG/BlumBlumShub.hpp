#ifndef MathLib_PRNG_BlumBlumShub_H
#define MathLib_PRNG_BlumBlumShub_H
#include "PRNG.hpp"
#include "../Math/Modulo.hpp"

namespace MathLib {
    template <typename T>
    struct BlumBlumShub : PRNG<T> {
        BlumBlumShub(const T& seed, const T& modulo) : PRNG<T>(seed), modulo(modulo) {
            EmptyBenchmark
        }
        [[nodiscard]] virtual T Random(void) override {
            StartBenchmark
            this->seed = Modulo<T>(this->seed * this->seed);
            ReturnFromBenchmark(this->seed / modulo);
        }

        private:
        T modulo;
    };
}

#endif