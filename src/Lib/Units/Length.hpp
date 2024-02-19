#ifndef Length_H
#define Length_H
#include "SIUnit.hpp"

template <typename T> struct Velocity;
template <typename T> struct Volume;
template <typename T> struct Area;
template <typename T> struct Time;
template <typename T>
struct Length : SIUnit<T> {
    MakeUnitConstructor(Length, Metre, T val, Length(val));
    MakeUnitConstructor(Length, Centimetre, T val, Metre(val / 100));
    MakeUnitConstructor(Length, Kilometre, T val, Metre(val * 1000));
    constexpr T operator/(Length<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Velocity<T> operator/(Time<T> time) {
        return Velocity<T>::MetrePerSecond(this->value / time.GetRaw());
    }
    constexpr Area<T> operator*(Length<T> other) {
        return Area<T>::SquareMetre(this->value * other.GetRaw());
    }
    constexpr Volume<T> operator*(Area<T> area) {
        return Volume<T>::CubicMetre(this->value * area.GetRaw());
    }

    private:
    constexpr Length(T val) : SIUnit<T>(val, "m") { }
};

#endif