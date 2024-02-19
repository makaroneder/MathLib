#ifndef Area_H
#define Area_H
#include "SIUnit.hpp"

template <typename T> struct Volume;
template <typename T> struct Length;
template <typename T>
struct Area : SIUnit<T> {
    MakeUnitConstructor(Area, SquareMetre, T val, Area(val));
    constexpr T operator/(Area<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Length<T> operator/(Length<T> len) {
        return Length<T>::Metre(this->value / len.GetRaw());
    }
    constexpr Volume<T> operator*(Length<T> len) {
        return Volume<T>::CubicMetre(this->value * len.GetRaw());
    }

    private:
    constexpr Area(T val) : SIUnit<T>(val, "m^2") { }
};

#endif