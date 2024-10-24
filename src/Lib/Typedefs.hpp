#ifndef Typedefs_H
#define Typedefs_H
#include <stdint.h>
#ifdef Freestanding
#include "CharBuffer.hpp"

/// @brief Signed version of size_t
typedef intptr_t ssize_t;
/// @brief Default collection implementation
/// @tparam T Type of data stored in the collection
template <typename T>
using Array = Buffer<T>;
/// @brief Default char collection implementation
using String = CharBuffer;
#else
#include "Libc/HostString.hpp"
#include "Libc/HostCollection.hpp"

/// @brief Default collection implementation
/// @tparam T Type of data stored in the collection
template <typename T>
using Array = HostCollection<T>;
/// @brief Default char collection implementation
using String = HostString;
#endif

#define SizeOfArray(arr) (sizeof(arr) / sizeof(arr[0]))
#define IsBetween(x, a, b) ((x) >= (a) && (x) <= (b))

/// @brief Default type for real numbers
typedef long double num_t;
/// @brief Default error tolerance
constexpr num_t eps = 1e-3;

num_t Abs(num_t x);
num_t Sqrt(num_t x);
num_t RandomFloat(void);

/// @brief |a - b| < eps
/// @tparam T Type of number
/// @param a a
/// @param b b
/// @param eps_ Maximum error tolerance
/// @return Equality
template <typename T>
constexpr bool FloatsEqual(const T& a, const T& b, const T& eps_ = eps) {
    return Abs(a - b) < eps_;
}
template <typename T>
T Sign(const T& x) {
    if (x < 0) return -1;
    else if (x > 0) return 1;
    else return 0;
}
template <typename T>
T LinearToGamma(const T& x) {
    return x > 0 ? Sqrt(x) : 0;
}
/// @brief Random number in range [min, max)
/// @tparam T Type of number
/// @param min Minimal value
/// @param max Maximal value
/// @return Random number
template <typename T>
T RandomNumber(const T& min, const T& max) {
    return RandomFloat() * (max - min) + min;
}
/// @brief Reverses bits of x
/// @tparam T Type of number
/// @param x Value to use
/// @return Value with reversed bits
template <typename T>
T BitReverse(const T& x, const uint8_t& bits = sizeof(T) * 8) {
    T ret = 0;
    for (uint8_t i = 0; i < bits; i++)
        ret |= !!(x & 1 << i) << (bits - i - 1);
    return ret;
}
/// @brief Swaps 2 values
/// @tparam T Type of value
/// @param a First value to swap
/// @param b Second value to swap
template <typename T>
void Swap(T& a, T& b) {
    const T tmp = a;
    a = b;
    b = tmp;
}
/// @brief Sorts specified array
/// @tparam T Type of data in array
/// @param array Array to be sorted
/// @return Sorted array
template <typename T>
Array<T> BubbleSort(const Array<T>& array) {
    Array<T> ret = array;
    while (true) {
        bool any = false;
        for (size_t i = 0; i < ret.GetSize() - 1; i++) {
            if (ret.At(i) < ret.At(i + 1)) {
                Swap<T>(ret.At(i), ret.At(i + 1));
                any = true;
            }
        }
        if (!any) break;
    }
    return ret;
}

#endif