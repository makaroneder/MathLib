#ifndef Mass_H
#define Mass_H
#include "SIUnit.hpp"

template <typename T> struct Acceleration;
template <typename T> struct Force;
template <typename T>
struct Mass : SIUnit<T> {
    MakeUnitConstructor(Mass, Kilogram, T val, Mass(val));
    MakeUnitConstructor(Mass, Gram, T val, Kilogram(val / 1000));
    constexpr T operator/(Mass<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Force<T> operator*(Acceleration<T> acceleration) {
        return Force<T>::Newton(this->value * acceleration.GetRaw());
    }

    private:
    constexpr Mass(T val) : SIUnit<T>(val, "kg") { }
};

#endif