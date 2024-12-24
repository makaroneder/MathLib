#ifndef MathLib_Typedefs_H
#define MathLib_Typedefs_H
#include <stdint.h>
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
#define StartBenchmark                              \
    MathLib::BenchmarkStart(__PRETTY_FUNCTION__);   \
    const MathLib::num_t benchmarkTimeStart = MathLib::benchmark ? MathLib::GetTime() : 0;
#define EndBenchmark MathLib::BenchmarkEnd(__PRETTY_FUNCTION__, benchmarkTimeStart);
#define ReturnFromBenchmark(ret) {  \
    EndBenchmark                    \
    return ret;                     \
}
#define EmptyBenchmark  \
    StartBenchmark      \
    EndBenchmark

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
    extern bool benchmark;

    num_t Abs(num_t x);
    num_t Sqrt(num_t x);
    num_t RandomFloat(void);
    num_t GetTime(void);

    bool BenchmarkStart(const char* function);
    bool BenchmarkEnd(const char* function, num_t time);
    /// @brief Makes array with specified single element
    /// @tparam T Type of element
    /// @param a Single element of created array
    /// @return Single element array
    template <typename T>
    Array<T> MakeArrayFromSingle(const T& a) {
        StartBenchmark
        Array<T> ret = Array<T>(1);
        ret.At(0) = a;
        ReturnFromBenchmark(ret);
    }
    /// @brief |a - b| < eps
    /// @tparam T Type of number
    /// @param a a
    /// @param b b
    /// @param eps_ Maximum error tolerance
    /// @return Equality
    template <typename T>
    constexpr bool FloatsEqual(const T& a, const T& b, const T& eps_ = eps) {
        StartBenchmark
        ReturnFromBenchmark(Abs(a - b) < eps_);
    }
    /// @brief Returns sign of specified number
    /// @tparam T Type of number
    /// @param x Number to return sign of
    /// @return Sign of specified number
    template <typename T>
    T Sign(const T& x) {
        StartBenchmark
        if (x < 0) ReturnFromBenchmark(-1)
        else if (x > 0) ReturnFromBenchmark(1)
        else ReturnFromBenchmark(0)
    }
    /// @brief Random number in range [min, max)
    /// @tparam T Type of number
    /// @param min Minimal value
    /// @param max Maximal value
    /// @return Random number
    template <typename T>
    T RandomNumber(const T& min, const T& max) {
        StartBenchmark
        ReturnFromBenchmark(RandomFloat() * (max - min) + min);
    }
    /// @brief Reverses bits of x
    /// @tparam T Type of number
    /// @param x Value to use
    /// @return Value with reversed bits
    template <typename T>
    T BitReverse(const T& x, const uint8_t& bits = sizeof(T) * 8) {
        StartBenchmark
        T ret = 0;
        for (uint8_t i = 0; i < bits; i++)
            ret |= !!(x & 1 << i) << (bits - i - 1);
        ReturnFromBenchmark(ret);
    }
    /// @brief Swaps 2 values
    /// @tparam T Type of value
    /// @param a First value to swap
    /// @param b Second value to swap
    template <typename T>
    void Swap(T& a, T& b) {
        StartBenchmark
        const T tmp = a;
        a = b;
        b = tmp;
        EndBenchmark
    }
    /// @brief Sorts specified array
    /// @tparam T Type of data in array
    /// @param array Array to be sorted
    /// @return Sorted array
    template <typename T>
    Array<T> BubbleSort(const Array<T>& array) {
        StartBenchmark
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
        ReturnFromBenchmark(ret);
    }
}

#endif
