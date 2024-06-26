#ifndef Typedefs_H
#define Typedefs_H
#include "Libc/HostString.hpp"
#include "Libc/HostCollection.hpp"
#include <functional>
#include <complex>

/// @brief Default collection implementation
/// @tparam T Type of data stored in the collection
template <typename T>
using Array = HostCollection<T>;
/// @brief Default char collection implementation
using String = HostString;

#define SizeOfArray(arr) (sizeof(arr) / sizeof(arr[0]))
#define IsBetween(x, a, b) ((x) >= (a) && (x) <= (b))

/// @brief Default type for real numbers
typedef long double num_t;
/// @brief Default type for complex numbers
typedef std::complex<num_t> complex_t;
/// @brief Default error tolerance
constexpr num_t eps = 1e-3;

num_t Abs(num_t x);
num_t Exp(num_t x);
complex_t Exp(complex_t x);
num_t RandomFloat(void);
String ToString(num_t x);

/// @brief |a - b| < eps
/// @tparam T Type of number
/// @param a a
/// @param b b
/// @param eps_ Maximum error tolerance
/// @return Equality
template <typename T>
constexpr bool FloatsEqual(T a, T b, T eps_ = eps) {
    return Abs(a - b) < eps_;
}
/// @brief Converts complex number to string
/// @tparam T Type of number
/// @param z Complex number
/// @return String representation
template <typename T>
constexpr String ComplexToString(std::complex<T> z) {
    return String('(') + ToString(z.real()) + ", " + ToString(z.imag()) + ')';
}
/// @brief 1 / (1 + e^-x)
/// @tparam T Type of number
/// @param x Sigmoid input
/// @return Return value
template <typename T>
constexpr T Sigmoid(T x) {
    const T one = 1;
    return one / (Exp(-x) + one);
}
/// @brief Random number in range [min, max]
/// @tparam T Type of number
/// @param min Minimal value
/// @param max Maximal value
/// @return Random number
template <typename T>
T RandomNumber(T min, T max) {
    return RandomFloat() * (max - min) + min;
}

#endif