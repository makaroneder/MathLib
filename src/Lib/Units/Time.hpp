#ifndef Time_H
#define Time_H
#include "SIUnit.hpp"

template <typename T> struct Acceleration;
template <typename T> struct Frequency;
template <typename T> struct Velocity;
template <typename T> struct Length;
template <typename T>
struct Time : SIUnit<T> {
    MakeUnitConstructor(Time, Second, T val, Time(val));
    MakeUnitConstructor(Time, Minute, T val, Second(val * 60));
    MakeUnitConstructor(Time, Hour, T val, Minute(val * 60));
    MakeUnitConstructor(Time, Day, T val, Hour(val * 24));
    constexpr T operator/(Time<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr Velocity<T> operator*(Acceleration<T> acceleration) {
        return Velocity<T>::MetrePerSecond(this->value * acceleration.GetRaw());
    }
    constexpr Length<T> operator*(Velocity<T> velocity) {
        return Length<T>::Metre(this->value * velocity.GetRaw());
    }
    constexpr T operator*(Frequency<T> frequency) {
        return this->value * frequency.GetRaw();
    }

    private:
    constexpr Time(T val) : SIUnit<T>(val, "s") { }
};

#endif