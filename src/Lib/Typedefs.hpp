#ifndef Typedefs_H
#define Typedefs_H
#include <cmath>
#include <typeinfo>

#define SizeOfArray(arr) (sizeof(arr) / sizeof(arr[0]))

/// @brief Defaults type for real numbers
typedef long double num_t;
/// @brief Maximal error in float comparison
static constexpr num_t eps = 0.01;

/// @brief Checks if T is float
/// @tparam T Type of number
/// @return Result
template <typename T>
constexpr bool IsFloat(void) {
    return typeid(T) == typeid(float) || typeid(T) == typeid(double) || typeid(T) == typeid(num_t);
}
/// @brief |a - b| < eps
/// @tparam T Type of number
/// @param a a
/// @param b b
/// @return Equality
template <typename T>
constexpr bool FloatsEqual(T a, T b) {
    return std::fabs(a - b) < eps;
}

#endif