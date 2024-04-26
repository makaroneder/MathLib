#ifndef ComplexPosition_H
#define ComplexPosition_H
#include "Matrix.hpp"
#include "Color.hpp"

template <typename T>
struct ComplexPosition {
    std::complex<T> position;
    std::complex<T> value;

    ComplexPosition(std::complex<T> pos, std::complex<T> val) {
        position = pos;
        value = val;
    }
    constexpr Matrix<T> GetPosition(void) const {
        return CreateVector<T>(position.real(), position.imag(), 0);
    }
    constexpr uint32_t GetColor(void) const {
        return GetRainbow<T>(CreateVector<T>(value.real(), value.imag(), 0).GetLength());
    }
};

#endif