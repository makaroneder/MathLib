#ifndef SIUnit_H
#define SIUnit_H
#include "Typedefs.hpp"
#include <ostream>

#define MakeUnitConstructor(name, unit, args, operation)    \
static constexpr name unit(args) {                          \
    return operation;                                       \
}
template <typename T>
struct SIUnit {
    constexpr T GetRaw(void) {
        return value;
    }
    constexpr std::string GetSymbol(void) {
        return symbol;
    }
    constexpr bool operator==(SIUnit<T> other) {
        return symbol == other.symbol && FloatsEqual<T>(value, other.value);
    }
    constexpr bool operator!=(SIUnit<T> other) {
        return symbol != other.symbol || !FloatsEqual<T>(value, other.value);
    }
    constexpr SIUnit<T> operator*(T num) {
        return SIUnit<T>(value * num, symbol);
    }
    constexpr SIUnit<T> operator/(T num) {
        return SIUnit<T>(value / num, symbol);
    }
    constexpr SIUnit<T> operator*=(T num) {
        value *= num;
        return *this;
    }
    constexpr SIUnit<T> operator/=(T num) {
        value /= num;
        return *this;
    }

    protected:
    constexpr SIUnit(T val, std::string sym) {
        value = val;
        symbol = sym;
    }
    T value;
    std::string symbol;
};
template <typename T>
std::ostream& operator<<(std::ostream& stream, SIUnit<T> unit) {
    return stream << unit.GetRaw() << unit.GetSymbol();
}

#endif