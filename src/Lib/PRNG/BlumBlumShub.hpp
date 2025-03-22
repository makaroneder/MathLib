#ifndef MathLib_PRNG_BlumBlumShub_H
#define MathLib_PRNG_BlumBlumShub_H
#include "PRNG.hpp"

namespace MathLib {
    template <typename T>
    struct BlumBlumShub : PRNG<T> {
        BlumBlumShub(const T& seed, const T& modulo) : PRNG<T>(seed), modulo(modulo) {}
        virtual T Random(void) override {
            this->seed *= this->seed;
            while (this->seed >= modulo) this->seed -= modulo;
            return this->seed / modulo;
        }

        private:
        T modulo;
    };
}

#endif