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
#define StartBenchmark if (!MathLib::IsCompileTimeEvaluated() && !MathLib::BenchmarkStart(__PRETTY_FUNCTION__)) MathLib::Panic("Failed to start benchmark");
#define EndBenchmark if (!MathLib::IsCompileTimeEvaluated() && !MathLib::BenchmarkEnd()) MathLib::Panic("Failed to end benchmark");
#define ReturnFromBenchmark(ret) {  \
    EndBenchmark                    \
    return ret;                     \
}
#define StartAndReturnFromBenchmark(ret)    \
    StartBenchmark                          \
    ReturnFromBenchmark(ret)
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

    [[noreturn]] void Panic(const char* str);
    [[nodiscard]] num_t Abs(num_t x);
    [[nodiscard]] num_t Sqrt(num_t x);
    [[nodiscard]] num_t RandomFloat(void);
    [[nodiscard]] num_t GetTime(void);
    [[nodiscard]] constexpr bool IsCompileTimeEvaluated(void) {
        #ifndef __MINGW32__
        return __builtin_is_constant_evaluated();
        #endif
        return true;
    }
    void StartBenchmarking(void);
    [[nodiscard]] bool BenchmarkStart(const char* function);
    [[nodiscard]] bool BenchmarkEnd(void);
    template <typename T>
    struct Tree;
    [[nodiscard]] Tree<num_t> StopBenchmarking(void);

    template <typename Arr, typename T>
    Arr ConvertCollection(const Sequence<T>& sequence) {
        Arr ret = Arr(sequence.GetSize());
        for (size_t i = 0; i < ret.GetSize(); i++) ret.At(i) = sequence.At(i);
        return ret;
    }
    template <typename T>
    Array<T> CollectionToArray(const Sequence<T>& sequence) {
        return ConvertCollection<Array<T>, T>(sequence);
    }
    String CollectionToString(const Sequence<char>& sequence);
    template <typename T>
    T Pop(Array<T>& array) {
        const size_t size = array.GetSize() - 1;
        const T ret = array.At(size);
        Array<T> tmp = Array<T>(0, size);
        for (size_t i = 0; i < size; i++) tmp.At(i) = array.At(i);
        array = tmp;
        return ret;
    }
    template <typename T>
    Array<T> MakeArray(T arg) {
        StartBenchmark
        Array<T> ret = Array<T>(1);
        ret.At(0) = arg;
        ReturnFromBenchmark(ret);
    }
    template <typename T, typename ...Args>
    Array<T> MakeArray(T arg, Args... args) {
        StartBenchmark
        Array<T> ret = MakeArray<T>(arg);
        Array<T> tmp = MakeArray<T>(args...);
        for (const T& x : tmp)
            if (!ret.Add(x)) ReturnFromBenchmark(Array<T>());
        ReturnFromBenchmark(ret);
    }
    /// @brief |a - b| < eps
    /// @tparam T Type of number
    /// @param a a
    /// @param b b
    /// @param eps_ Maximum error tolerance
    /// @return Equality
    template <typename T>
    [[nodiscard]] bool FloatsEqual(const T& a, const T& b, const T& eps_ = eps) {
        StartAndReturnFromBenchmark(Abs(a - b) < eps_);
    }
    /// @brief Returns sign of specified number
    /// @tparam T Type of number
    /// @param x Number to return sign of
    /// @return Sign of specified number
    template <typename T>
    [[nodiscard]] constexpr T Sign(const T& x) {
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
    [[nodiscard]] T RandomNumber(const T& min, const T& max) {
        StartAndReturnFromBenchmark(RandomFloat() * (max - min) + min);
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
    [[nodiscard]] constexpr T BitReverse(const T& x, const uint8_t& bits = sizeof(T) * 8) {
        StartBenchmark
        T ret = 0;
        for (uint8_t i = 0; i < bits; i++)
            ret |= !!(x & 1 << i) << (bits - i - 1);
        ReturnFromBenchmark(ret);
    }
    /// @brief Sorts specified array
    /// @tparam T Type of data in array
    /// @param array Array to be sorted
    /// @return Sorted array
    template <typename T>
    [[nodiscard]] Array<T> BubbleSort(const Sequence<T>& array, bool largestFirst) {
        StartBenchmark
        Array<T> ret = CollectionToArray<T>(array);
        while (true) {
            bool any = false;
            for (size_t i = 0; i < ret.GetSize() - 1; i++) {
                const bool x = ret.At(i) < ret.At(i + 1);
                if ((largestFirst && x) || (!largestFirst && !x)) {
                    Swap<T>(ret.At(i), ret.At(i + 1));
                    any = true;
                }
            }
            if (!any) break;
        }
        ReturnFromBenchmark(ret);
    }
}
MathLib::String operator""_M(const char* str, size_t size);
MathLib::String operator""_M(char chr);

#endif