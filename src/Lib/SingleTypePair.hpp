#ifndef MathLib_SingleTypePair_H
#define MathLib_SingleTypePair_H
#include "Pair.hpp"

namespace MathLib {
    template <typename T>
    struct SingleTypePair : Pair<T, T> {
        SingleTypePair(void) {
            EmptyBenchmark
        }
        SingleTypePair(const T& first, const T& second) : Pair<T, T>(first, second) {
            EmptyBenchmark
        }
        T operator[](bool i) const {
            return i ? this->second : this->first;
        }
        T& operator[](bool i) {
            return i ? this->second : this->first;
        }
    };
}

#endif