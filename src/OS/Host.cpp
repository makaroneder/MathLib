#include "Halt.hpp"
#include "E9.hpp"
#include <Host.hpp>
#include <String.hpp>

[[noreturn]] void Panic(const char* str) {
    E9().Write(str);
    Halt();
}
num_t StringToNumber(String str) {
    // TODO:
    (void)str;
    return 0;
}
String ToString(num_t x) {
    ssize_t ix = (ssize_t)x;
    if (x == ix) {
        bool neg = false;
        if (ix < 0) {
            neg = true;
            ix *= -1;
        }
        return (neg ? String("-") : String()) + ToString(ix, 10);
    }
    else {
        // TODO:
        return String();
    }
}
num_t GetTime(void) {
    // TODO:
    return 0;
}
size_t GetThreadCount(void) {
    // TODO:
    return 0;
}
Thread* AllocThread(void) {
    // TODO:
    return nullptr;
}
void DeallocThread(Thread* thread) {
    // TODO:
    (void)thread;
}
num_t MakeNaN(void) {
    // TODO:
    return 0;
}
num_t MakeInf(void) {
    // TODO:
    return 0;
}
num_t RandomFloat(void) {
    // TODO:
    return 0;
}
num_t Abs(complex_t x) {
    return Sqrt(Pow(complex_t(x.GetReal(), 0), 2) + Pow(complex_t(x.GetImaginary(), 0), 2)).GetReal();
}
complex_t Pow(complex_t x, complex_t y) {
    // TODO:
    (void)x;
    (void)y;
    return complex_t();
}
complex_t NaturalLog(complex_t x) {
    // TODO:
    (void)x;
    return complex_t();
}
bool IsNaN(num_t x) {
    // TODO:
    (void)x;
    return false;
}
bool IsInf(num_t x) {
    // TODO:
    (void)x;
    return false;
}
num_t Round(num_t x) {
    // TODO:
    (void)x;
    return 0;
}
num_t Floor(num_t x) {
    // TODO:
    (void)x;
    return 0;
}
complex_t Sin(complex_t x) {
    // TODO:
    (void)x;
    return complex_t();
}
complex_t InversedSin(complex_t x) {
    // TODO:
    (void)x;
    return complex_t();
}
num_t InversedTan2(num_t y, num_t x) {
    // TODO:
    (void)y;
    (void)x;
    return 0;
}