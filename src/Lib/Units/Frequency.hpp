#ifndef Frequency_H
#define Frequency_H
#include "SIUnit.hpp"

template <typename T> struct Time;
template <typename T>
struct Frequency : SIUnit<T> {
    MakeUnitConstructor(Frequency, Hertz, T val, Frequency(val));
    constexpr T operator/(Frequency<T> other) {
        return this->value / other.GetRaw();
    }
    constexpr T operator*(Time<T> time) {
        return this->value * time.GetRaw();
    }

    private:
    constexpr Frequency(T val) : SIUnit<T>(val, "Hz") { }
};


#endif