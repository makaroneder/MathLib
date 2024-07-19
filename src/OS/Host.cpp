#include "Time.hpp"
#include "Halt.hpp"
#include "E9.hpp"
#include <Host.hpp>
#include <String.hpp>

[[noreturn]] void Panic(const char* str) {
    asm volatile("cli");
    E9().Write(str);
    Halt();
}
num_t StringToNumber(String str) {
    // TODO: important
    (void)str;
    return 0;
}
String ToString(num_t x) {
    bool neg = false;
    if (x < 0) {
        neg = true;
        x *= -1;
    }
    String ret = String(neg ? "-" : "") + ToString((ssize_t)x, 10);
    if ((ssize_t)x == x) return ret;
    ret += '.';
    x -= (ssize_t)x;
    for (size_t i = 0; i < 10; i++) {
        x *= 10;
        ret += '0' + x;
        x -= (ssize_t)x;
    }
    return ret;
}
num_t GetTime(void) {
    return mainTimer ? mainTimer->GetTime() : 0;
}
size_t GetThreadCount(void) {
    // TODO: not used in library
    return 0;
}
Thread* AllocThread(void) {
    // TODO: not used in library
    return nullptr;
}
void DeallocThread(Thread* thread) {
    // TODO: not used in library
    (void)thread;
}
num_t MakeNaN(void) {
    return __builtin_nanl("");
}
num_t MakeInf(void) {
    return __builtin_infl();
}
num_t RandomFloat(void) {
    // TODO: almost not used in library
    return 0;
}
num_t Abs(complex_t x) {
    return Sqrt(Pow(x.GetReal(), 2) + Pow(x.GetImaginary(), 2));
}
complex_t Pow(complex_t x, complex_t y) {
    return Exp(y * NaturalLog(x));
}
complex_t NaturalLog(complex_t x) {
    // TODO: important
    (void)x;
    return complex_t();
}
bool IsNaN(num_t x) {
    return x != x;
}
bool IsInf(num_t x) {
    const num_t infinity = MakeInf();
    return x == infinity || x == -infinity;
}
num_t Exp(num_t x) {
    num_t sum = 1;
    num_t term = 1;
    size_t n = 1;
    while (Abs(term) > eps) {
        term *= x / n++;
        sum += term;
    }
    return sum;
}
num_t Round(num_t x) {
    // TODO: almost not used in library
    (void)x;
    return 0;
}
num_t Floor(num_t x) {
    const ssize_t ix = x;
    return x == ix ? ix : (ix - 1);
}
complex_t Sin(complex_t x) {
    // TODO: important
    (void)x;
    return complex_t();
}
complex_t InversedSin(complex_t x) {
    // TODO: important
    (void)x;
    return complex_t();
}
num_t InversedTan2(num_t y, num_t x) {
    // TODO: almost not used in library
    (void)y;
    (void)x;
    return 0;
}