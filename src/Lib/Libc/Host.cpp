#ifndef Freestanding
#include "../Host.hpp"
#include "HostThread.hpp"
#include <complex>
#include <thread>
#define ToStdComplex(x) std::complex<num_t>(x.GetReal(), x.GetImaginary())
#define FromStdComplex(x) complex_t(x.real(), x.imag())

size_t threadCount = std::thread::hardware_concurrency();
std::ostream& operator<<(std::ostream& stream, const String& string) {
    return stream << string.GetValue();
}
std::ostream& operator<<(std::ostream& stream, const Printable& printable) {
    return stream << printable.ToString();
}
[[noreturn]] void Panic(const char* str) {
    throw std::runtime_error(str);
}
num_t StringToNumber(String str) {
    return std::stold(str.GetValue());
}
String ToString(num_t x) {
    const ssize_t ix = (ssize_t)x;
    if ((num_t)ix == x) return std::to_string(ix);
    else return std::to_string(x);
}
num_t GetTime(void) {
    return (num_t)clock() / CLOCKS_PER_SEC;
}
size_t GetThreadCount(void) {
    return threadCount;
}
Thread* AllocThread(void) {
    if (!threadCount) return nullptr;
    threadCount--;
    return new HostThread();
}
void DeallocThread(Thread* thread) {
    threadCount++;
    delete thread;
}
num_t MakeNaN(void) {
    return NAN;
}
num_t MakeInf(void) {
    return INFINITY;
}
num_t RandomFloat(void) {
    return (num_t)rand() / RAND_MAX;
}
num_t Abs(complex_t x) {
    return std::abs(ToStdComplex(x));
}
complex_t Pow(complex_t x, complex_t y) {
    return FromStdComplex(std::pow(ToStdComplex(x), ToStdComplex(y)));
}
num_t Cbrt(num_t x) {
    return std::cbrt(x);
}
complex_t NaturalLog(complex_t x) {
    return FromStdComplex(std::log(ToStdComplex(x)));
}
bool IsNaN(num_t x) {
    return std::isnan(x);
}
bool IsInf(num_t x) {
    return std::isinf(x);
}
num_t Exp(num_t x) {
    return std::exp(x);
}
num_t Round(num_t x) {
    return std::round(x);
}
num_t Floor(num_t x) {
    return std::floor(x);
}
complex_t Sin(complex_t x) {
    return FromStdComplex(std::sin(ToStdComplex(x)));
}
complex_t InversedSin(complex_t x) {
    return FromStdComplex(std::asin(ToStdComplex(x)));
}
num_t InversedTan2(num_t y, num_t x) {
    return std::atan2(y, x);
}

#endif