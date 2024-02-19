#ifndef Volume_H
#define Volume_H
#include "SIUnit.hpp"

template <typename T> struct Length;
template <typename T> struct Area;
template <typename T>
struct Volume : SIUnit<T> {
    MakeUnitConstructor(Volume, CubicMetre, T val, Volume(val));
    constexpr T operator/(Volume<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Length<T> operator/(Area<T> area) {
        return Length<T>::Metre(this->value / area.GetRaw());
    }
    constexpr Area<T> operator/(Length<T> len) {
        return Area<T>::SquareMetre(this->value / len.GetRaw());
    }

    private:
    constexpr Volume(T val) : SIUnit<T>(val, "m^3") { }
};

#endif