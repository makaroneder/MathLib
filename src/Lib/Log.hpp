#ifndef Log_H
#define Log_H
#include "Host.hpp"

/// @brief log_b x = log x / log b
/// @tparam T Type of number
/// @param b Base of the logarithm
/// @param x Value of the logarithm
/// @return Logarithm of specified base and number
template <typename T>
constexpr T Log(T b, T x) {
    return NaturalLog(x) / NaturalLog(b);
}

#endif