#include "Arch/Arch.hpp"
#include "Time.hpp"
#include <Host.hpp>
#include <Logger.hpp>
#include <String.hpp>
#include <Math/Trigonometry.hpp>

num_t RealNaturalLog(num_t x) {
    if (x <= 0) return MakeNaN();
    num_t logx = 0;
    while (x >= 2) {
        x /= 2;
        logx += ln2;
    }
    num_t result = (x - 1) / (x + 1);
    num_t numerator = result;
    const num_t tmp = result * result;
    size_t i = 1;
    while (Abs(numerator) > eps) {
        numerator *= tmp;
        i += 2;
        result += numerator / i;
    }
    return logx + result * 2;
}
num_t RealSqrt(num_t x) {
    if (x < 0) return MakeNaN();
    if (!x) return 0;
    num_t guess = x;
    num_t prev;
    do {
        prev = guess;
        guess = (guess + x / guess) / 2;
    } while (Abs(guess - prev) > eps);
    return guess;
}
num_t RealInversedTan(num_t x) {
    if (Abs(x) > 1) {
        if (x > 0) return pi / 2 - RealInversedTan(1 / x);
        else return -pi / 2 - RealInversedTan(1 / x);
    }
    else {
        num_t result = 0;
        num_t term = x;
        const num_t tmp = x * x;
        size_t n = 1;
        while (Abs(term) > eps) {
            result += term;
            term *= -tmp * (2 * n - 1) / (2 * n + 1);
            n++;
        }
        return result;
    }
}
complex_t ComplexExp(complex_t x) {
    complex_t term = complex_t(1, 0);
    complex_t sum = term;
    size_t n = 1;
    while (Abs(term) > eps) {
        term *= complex_t(0, x.GetImaginary()) / n;
        sum += term;
        n++;
    }
    return sum * Exp(x.GetReal());
}
[[noreturn]] void Panic(const char* str) {
    if (LogString(str)) LogChar('\n');
    ArchPanic();
}
num_t StringToNumber(String str) {
    size_t i = 0;
    num_t ret = 0;
    size_t mult = 1;
    while (i < str.GetSize() && str.At(i) != '.') {
        ret = ret * mult + str.At(i++) - '0';
        mult *= 10;
    }
    if (i < str.GetSize() && str.At(i) == '.') {
        i++;
        mult = 10;
        while (i < str.GetSize()) {
            ret += num_t(str.At(i++) - '0') / mult;
            mult *= 10;
        }
    }
    return ret;
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
    return 0;
}
Thread* AllocThread(void) {
    return nullptr;
}
void DeallocThread(Thread* thread) {
    (void)thread;
}
num_t MakeNaN(void) {
    return __builtin_nanl("");
}
num_t MakeInf(void) {
    return __builtin_infl();
}
uint32_t rand = 1;
num_t RandomFloat(void) {
    rand = rand * 1103515245 + 12345;
    return (num_t)((rand / (UINT16_MAX + 1)) % ((UINT16_MAX + 1) / 2)) / ((UINT16_MAX + 1) / 2);
}
num_t Abs(complex_t x) {
    return RealSqrt(x.GetReal() * x.GetReal() + x.GetImaginary() * x.GetImaginary());
}
complex_t Pow(complex_t x, complex_t y) {
    return ComplexExp(y * NaturalLog(x));
}
complex_t NaturalLog(complex_t x) {
    return complex_t(RealNaturalLog(Abs(x)), x.GetArgument());
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
    const ssize_t ix = x;
    x -= ix;
    if (Abs(x) >= 0.5) return ix + Sign<num_t>(x);
    else return ix;
}
num_t Floor(num_t x) {
    const ssize_t ix = x;
    return x < 0 ? (ix - 1) : ix;
}
complex_t Sin(complex_t x) {
    complex_t term = x;
    complex_t sum = term;
    const complex_t tmp = x * x;
    size_t n = 1;
    while (Abs(term) > eps) {
        term *= -tmp / ((2 * n) * (2 * n + 1));
        sum += term;
        n++;
    }
    return sum;
}
complex_t InversedSin(complex_t x) {
    return complex_t(0, 1) * NaturalLog(Sqrt(-x * x + 1) - x * complex_t(0, 1));
}
num_t InversedTan2(num_t y, num_t x) {
    if (x > 0) return RealInversedTan(y / x);
    else if (x < 0) {
        if (y >= 0) return RealInversedTan(y / x) + pi;
        else return RealInversedTan(y / x) - pi;
    }
    else if (y > 0) return pi / 2;
    else if (y < 0) return -pi / 2;
    else return MakeNaN();
}