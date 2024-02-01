#ifndef Log_H
#define Log_H
#include <cmath>

/// @brief log_b x = log x / log b
/// @tparam T Type of number
/// @param b Base of the logarithm
/// @param x Value of the logarithm
/// @return Logarithm of specified base and number
template <typename T>
T Log(T b, T x) {
    return std::log2(x) / std::log2(b);
}

#endif