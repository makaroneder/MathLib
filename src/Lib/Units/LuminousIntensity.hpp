#ifndef LuminousIntensity_H
#define LuminousIntensity_H
#include "SIUnit.hpp"

template <typename T>
struct LuminousIntensity : SIUnit<T> {
    MakeUnitConstructor(LuminousIntensity, Candela, T val, LuminousIntensity(val));
    constexpr T operator/(LuminousIntensity<T> other) {
        return this->value / other.GetRaw();
    }

    private:
    constexpr LuminousIntensity(T val) : SIUnit<T>(val, "cd") { }
};

#endif