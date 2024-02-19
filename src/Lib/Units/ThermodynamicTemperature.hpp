#ifndef ThermodynamicTemperature_H
#define ThermodynamicTemperature_H
#include "SIUnit.hpp"

template <typename T>
struct ThermodynamicTemperature : SIUnit<T> {
    MakeUnitConstructor(ThermodynamicTemperature, Kelvin, T val, ThermodynamicTemperature(val));
    constexpr T operator/(ThermodynamicTemperature<T> other) {
        return this->value / other.GetRaw();
    }

    private:
    constexpr ThermodynamicTemperature(T val) : SIUnit<T>(val, "K") { }
};

#endif