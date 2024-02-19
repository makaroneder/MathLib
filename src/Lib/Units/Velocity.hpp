#ifndef Velocity_H
#define Velocity_H
#include "SIUnit.hpp"

template <typename T> struct Acceleration;
template <typename T> struct Length;
template <typename T> struct Time;
template <typename T>
struct Velocity : SIUnit<T> {
    MakeUnitConstructor(Velocity, MetrePerSecond, T val, Velocity(val));
    MakeUnitConstructor(Velocity, KilometrePerHour, T val, MetrePerSecond(val * 1000 / 3600));
    constexpr T operator/(Velocity<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Acceleration<T> operator/(Time<T> time) {
        return Acceleration<T>::MetrePerSecondSquared(this->value / time.GetRaw());
    }
    constexpr Length<T> operator*(Time<T> time) {
        return Length<T>::Metre(this->value * time.GetRaw());
    }

    private:
    constexpr Velocity(T val) : SIUnit<T>(val, "m/s") { }
};

#endif