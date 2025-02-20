#ifndef Freestanding
#include "../Host.hpp"
#include "HostThread.hpp"
#include <complex>
#include <time.h>
#ifndef __MINGW32__
#include <thread>
#endif
#define ToStdComplex(x) std::complex<num_t>(x.GetReal(), x.GetImaginary())
#define FromStdComplex(x) complex_t(x.real(), x.imag())

namespace MathLib {
    const num_t nan = NAN;
    const num_t infinity = INFINITY;

    #ifdef __MINGW32__
    size_t GetThreadCount(void) {
        StartAndReturnFromBenchmark(0);
    }
    Thread* AllocThread(void) {
        StartAndReturnFromBenchmark(nullptr);
    }
    void DeallocThread(Thread*) {
        EmptyBenchmark
    }
    #else
    size_t threadCount = std::thread::hardware_concurrency();
    size_t GetThreadCount(void) {
        StartAndReturnFromBenchmark(threadCount);
    }
    Thread* AllocThread(void) {
        StartBenchmark
        if (!threadCount) ReturnFromBenchmark(nullptr);
        threadCount--;
        ReturnFromBenchmark(new HostThread());
    }
    void DeallocThread(Thread* thread) {
        StartBenchmark
        threadCount++;
        delete thread;
        EndBenchmark
    }
    #endif
    std::ostream& operator<<(std::ostream& stream, const String& string) {
        StartAndReturnFromBenchmark(stream << string.GetValue());
    }
    std::ostream& operator<<(std::ostream& stream, const Printable& printable) {
        StartAndReturnFromBenchmark(stream << printable.ToString());
    }
    [[noreturn]] void Panic(const char* str) {
        throw std::runtime_error(str);
    }
    num_t StringToNumber(String str) {
        StartAndReturnFromBenchmark(std::stold(str.GetValue()));
    }
    String ToString(num_t x) {
        StartBenchmark
        const ssize_t ix = (ssize_t)x;
        ReturnFromBenchmark((num_t)ix == x ? std::to_string(ix) : std::to_string(x));
    }
    num_t GetTime(void) {
        StartAndReturnFromBenchmark((num_t)clock() / CLOCKS_PER_SEC);
    }
    num_t RandomFloat(void) {
        StartAndReturnFromBenchmark((num_t)rand() / RAND_MAX);
    }
    num_t Abs(complex_t x) {
        StartAndReturnFromBenchmark(std::abs(ToStdComplex(x)));
    }
    complex_t Pow(complex_t x, complex_t y) {
        StartAndReturnFromBenchmark(FloatsEqual<num_t>(x.ToReal(), 0) ? complex_t(FloatsEqual<num_t>(y.ToReal(), 0), 0) : FromStdComplex(std::pow(ToStdComplex(x), ToStdComplex(y))));
    }
    complex_t NaturalLog(complex_t x) {
        StartAndReturnFromBenchmark(FromStdComplex(std::log(ToStdComplex(x))));
    }
    bool IsNaN(num_t x) {
        StartAndReturnFromBenchmark(std::isnan(x));
    }
    bool IsInf(num_t x) {
        StartAndReturnFromBenchmark(std::isinf(x));
    }
    num_t Exp(num_t x) {
        StartAndReturnFromBenchmark(std::exp(x));
    }
    num_t Round(num_t x) {
        StartAndReturnFromBenchmark(std::round(x));
    }
    num_t Floor(num_t x) {
        StartAndReturnFromBenchmark(std::floor(x));
    }
    complex_t Sin(complex_t x) {
        StartAndReturnFromBenchmark(FromStdComplex(std::sin(ToStdComplex(x))));
    }
    complex_t InversedSin(complex_t x) {
        StartAndReturnFromBenchmark(FromStdComplex(std::asin(ToStdComplex(x))));
    }
    num_t InversedTan2(num_t y, num_t x) {
        StartAndReturnFromBenchmark(std::atan2(y, x));
    }
}

#endif