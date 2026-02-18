#ifndef MathLib_Host_H
#define MathLib_Host_H
#include "Thread.hpp"
#include "Utils.hpp"

namespace MathLib {
    extern const num_t nan;
    extern const num_t infinity;

    [[noreturn]] void Panic(const char* str);
    [[nodiscard]] num_t StringToNumber(const Sequence<char>& str);
    [[nodiscard]] String ToString(num_t x);
    [[nodiscard]] num_t GetTime(void);
    [[nodiscard]] size_t GetThreadCount(void);
    [[nodiscard]] Thread* AllocThread(void);
    void DeallocThread(Thread* thread);
    [[nodiscard]] num_t RandomFloat(void);
    [[nodiscard]] num_t Abs(complex_t x);
    [[nodiscard]] complex_t Pow(complex_t x, complex_t y);
    [[nodiscard]] complex_t NaturalLog(complex_t x);
    [[nodiscard]] bool IsNaN(num_t x);
    [[nodiscard]] bool IsInf(num_t x);
    [[nodiscard]] num_t Exp(num_t x);
    [[nodiscard]] num_t Round(num_t x);
    [[nodiscard]] num_t Floor(num_t x);
    [[nodiscard]] complex_t Sin(complex_t x);
    [[nodiscard]] complex_t InversedSin(complex_t x);
    [[nodiscard]] num_t InversedTan2(num_t y, num_t x);
}

#endif