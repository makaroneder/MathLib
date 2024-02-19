#ifndef Acceleration_H
#define Acceleration_H
#include "SIUnit.hpp"

template <typename T> struct Velocity;
template <typename T> struct Force;
template <typename T> struct Mass;
template <typename T> struct Time;
template <typename T>
struct Acceleration : SIUnit<T> {
    MakeUnitConstructor(Acceleration, MetrePerSecondSquared, T val, Acceleration(val));
    constexpr T operator/(Acceleration<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Force<T> operator*(Mass<T> mass) {
        return Force<T>::Newton(this->value * mass.GetRaw());
    }
    constexpr Velocity<T> operator*(Time<T> time) {
        return Velocity<T>::MetrePerSecond(this->value * time.GetRaw());
    }

    private:
    constexpr Acceleration(T val) : SIUnit<T>(val, "m/s^2") { }
};

#endif