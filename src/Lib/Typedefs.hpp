#ifndef MathLib_Typedefs_H
#define MathLib_Typedefs_H
#include "Swap.hpp"
#ifdef Freestanding
#include "CharBuffer.hpp"

/// @brief Signed version of size_t
using ssize_t = intptr_t;
#else
#include "Libc/HostString.hpp"
#include "Libc/HostCollection.hpp"
#endif
#define SizeOfArray(arr) (sizeof(arr) / sizeof(arr[0]))
#define IsBetween(x, a, b) ((x) >= (a) && (x) <= (b))

namespace MathLib {
    #ifdef Freestanding
    /// @brief Default collection implementation
    /// @tparam T Type of data stored in the collection
    template <typename T>
    using Array = Buffer<T>;
    /// @brief Default char collection implementation
    using String = CharBuffer;
    #else
    /// @brief Default collection implementation
    /// @tparam T Type of data stored in the collection
    template <typename T>
    using Array = HostCollection<T>;
    /// @brief Default char collection implementation
    using String = HostString;
    #endif

    /// @brief Default type for real numbers
    using num_t = long double;
    /// @brief Default error tolerance
    extern num_t eps;

    [[nodiscard]] num_t Abs(num_t x);
    [[nodiscard]] num_t Sqrt(num_t x);
    [[nodiscard]] num_t RandomFloat(void);
    [[nodiscard]] num_t GetTime(void);

    template <typename Arr, typename T>
    Arr ConvertCollection(const Sequence<T>& sequence) {
        const size_t size = sequence.GetSize();
        Arr ret = Arr(size);
        for (size_t i = 0; i < size; i++) ret.AtUnsafe(i) = sequence.AtUnsafe(i);
        return ret;
    }
    template <typename T>
    Array<T> CollectionToArray(const Sequence<T>& sequence) {
        return ConvertCollection<Array<T>, T>(sequence);
    }
    String CollectionToString(const Sequence<char>& sequence);
    template <typename T>
    Array<T> MakeArray(T arg) {
        Array<T> ret = Array<T>(1);
        ret.AtUnsafe(0) = arg;
        return ret;
    }
    template <typename T, typename... Args>
    Array<T> MakeArray(T arg, Args... args) {
        Array<T> ret = MakeArray<T>(arg);
        Array<T> tmp = MakeArray<T>(args...);
        for (const T& x : tmp)
            if (!ret.Add(x)) return Array<T>();
        return ret;
    }
    /// @brief |a - b| < eps
    /// @tparam T Type of number
    /// @param a a
    /// @param b b
    /// @param eps_ Maximum error tolerance
    /// @return Equality
    template <typename T>
    [[nodiscard]] bool FloatsEqual(const T& a, const T& b, const T& eps_ = eps) {
        return Abs(a - b) < eps_;
    }
    /// @brief Returns sign of specified number
    /// @tparam T Type of number
    /// @param x Number to return sign of
    /// @return Sign of specified number
    template <typename T>
    [[nodiscard]] constexpr T Sign(const T& x) {
        if (x < 0) return -1;
        if (x > 0) return 1;
        return 0;
    }
    /// @brief Random number in range [min, max]
    /// @tparam T Type of number
    /// @param min Minimal value
    /// @param max Maximal value
    /// @return Random number
    template <typename T>
    [[nodiscard]] T RandomNumber(const T& min, const T& max) {
        return RandomFloat() * (max - min) + min;
    }
    template <typename T>
    [[nodiscard]] T RelativeError(const T& x, const T& expected) {
        return FloatsEqual<T>(expected, 0) ? !FloatsEqual<T>(x, 0) : Abs(1 - x / expected);
    }
    /// @brief Reverses bits of x
    /// @tparam T Type of number
    /// @param x Value to use
    /// @return Value with reversed bits
    template <typename T>
    [[nodiscard]] constexpr T BitReverse(const T& x, uint8_t bits = sizeof(T) * 8) {
        T ret = 0;
        for (uint8_t i = 0; i < bits; i++)
            ret |= !!(x & 1 << i) << (bits - i - 1);
        return ret;
    }
    template <typename T>
    T CircularLeftShift(const T& x, uint8_t n, uint8_t bits = sizeof(T) * 8) {
        return (x << n) | (x >> (bits - n));
    }
    /// @brief Sorts specified array
    /// @tparam T Type of data in array
    /// @param array Array to be sorted
    /// @return Sorted array
    template <typename T>
    [[nodiscard]] Array<T> StalinSort(const Sequence<T>& array, const Function<bool, T, T>& compare) {
        const size_t size = array.GetSize();
        if (!size) return Array<T>();
        Array<T> ret = MakeArray<T>(array.At(0));
        for (size_t i = 1; i < size; i++) {
            const T tmp = array.At(i);
            if (!compare(ret.AtUnsafe(ret.GetSize() - 1), tmp) && !ret.Add(tmp)) return Array<T>();
        }
        return ret;
    }
    [[nodiscard]] uint8_t StringToU4(char chr);
    [[nodiscard]] uint8_t StringToU8(char a, char b);
}
MathLib::String operator""_M(const char* str, size_t size);
MathLib::String operator""_M(char chr);

#endif