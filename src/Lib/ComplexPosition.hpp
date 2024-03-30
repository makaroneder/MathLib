#ifndef ComplexPosition_H
#define ComplexPosition_H
#include "Typedefs.hpp"

template <typename T>
struct ComplexPosition {
    std::complex<T> position;
    std::complex<T> value;

    ComplexPosition(std::complex<T> pos, std::complex<T> val) {
        position = pos;
        value = val;
    }
};

#endif