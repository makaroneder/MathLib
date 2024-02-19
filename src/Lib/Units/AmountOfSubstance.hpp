#ifndef AmountOfSubstance_H
#define AmountOfSubstance_H
#include "SIUnit.hpp"

template <typename T>
struct AmountOfSubstance : SIUnit<T> {
    MakeUnitConstructor(AmountOfSubstance, Mole, T val, AmountOfSubstance(val));
    constexpr T operator/(AmountOfSubstance<T> other) {
        return this->value / other.GetRaw();
    }

    private:
    constexpr AmountOfSubstance(T val) : SIUnit<T>(val, "mol") { }
};

#endif