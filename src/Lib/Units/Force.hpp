#ifndef Force_H
#define Force_H
#include "SIUnit.hpp"

template <typename T> struct Acceleration;
template <typename T> struct Pressure;
template <typename T> struct Area;
template <typename T> struct Mass;
template <typename T>
struct Force : SIUnit<T> {
    MakeUnitConstructor(Force, Newton, T val, Force(val));
    constexpr T operator/(Force<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Pressure<T> operator/(Area<T> area) {
        return Pressure<T>::Pascal(this->value / area.GetRaw());
    }
    constexpr Acceleration<T> operator/(Mass<T> mass) {
        return Acceleration<T>::MetrePerSecondSquared(this->value / mass.GetRaw());
    }
    constexpr Mass<T> operator/(Acceleration<T> acceleration) {
        return Mass<T>::Kilogram(this->value / acceleration.GetRaw());
    }

    private:
    constexpr Force(T val) : SIUnit<T>(val, "N") { }
};

#endif