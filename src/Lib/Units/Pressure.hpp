#ifndef Pressure_H
#define Pressure_H
#include "SIUnit.hpp"

template <typename T>
struct Pressure : SIUnit<T> {
    MakeUnitConstructor(Pressure, Pascal, T val, Pressure(val));
    constexpr T operator/(Pressure<T> other) {
        return this->value / other.GetRaw();
    }

    private:
    constexpr Pressure(T val) : SIUnit<T>(val, "Pa") { }
};

#endif