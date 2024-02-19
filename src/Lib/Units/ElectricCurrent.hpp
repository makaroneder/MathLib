#ifndef ElectricCurrent_H
#define ElectricCurrent_H
#include "SIUnit.hpp"

template <typename T>
struct ElectricCurrent : SIUnit<T> {
    MakeUnitConstructor(ElectricCurrent, Ampere, T val, ElectricCurrent(val));
    constexpr T operator/(ElectricCurrent<T> other) {
        return this->value / other.GetRaw();
    }

    private:
    constexpr ElectricCurrent(T val) : SIUnit<T>(val, "A") { }
};

#endif