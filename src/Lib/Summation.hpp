#ifndef Summation_H
#define Summation_H
#include <functional>

/// @brief eq(start) + ... + eq(end)
/// @tparam T Type of number
/// @param start First value of the sequence
/// @param end Last value of the sequence (we are not interested in it)
/// @param eq Function wich we are summing
/// @return Sum of every number given by functions in the specified sequence
template <typename T>
T Summation(T start, T end, std::function<T(T)> eq) {
    T ret = 0;
    for (T i = start; i < end; i++) ret += eq(i);
    return ret;
}

#endif