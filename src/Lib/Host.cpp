#include "Host.hpp"
#include "Constants.hpp"

#define ComplexToReal(x)                                                \
    const complex_t complex = x;                                        \
    return FloatsEqual<num_t>(complex.imag(), 0) ? complex.real() : NAN

bool IsUpper(char chr) {
    return chr >= 'A' && chr <= 'Z';
}
bool IsLower(char chr) {
    return chr >= 'a' && chr <= 'z';
}
bool IsAlpha(char chr) {
    return IsUpper(chr) || IsLower(chr);
}
bool IsDigit(char chr) {
    return chr >= '0' && chr <= '9';
}
bool IsHexDigit(char chr) {
    return IsDigit(chr) || (chr >= 'a' && chr <= 'f') || (chr >= 'A' && chr <= 'F');
}
bool IsAlphaDigit(char chr) {
    return IsAlpha(chr) || IsDigit(chr);
}
num_t Abs(num_t x) {
    return Abs(complex_t(x, 0));
}
num_t Max(num_t x, num_t y) {
    return (x <= y) ? y : x;
}
num_t Pow(num_t x, num_t y) {
    ComplexToReal(Pow(complex_t(x, 0), complex_t(y, 0)));
}
complex_t Sqrt(complex_t x) {
    return Pow(x, 0.5);
}
num_t Sqrt(num_t x) {
    ComplexToReal(Sqrt(complex_t(x, 0)));
}
#ifdef Freestanding
num_t Cbrt(num_t x) {
    return Pow(x, 1 / 3);
}
#endif
num_t NaturalLog(num_t x) {
    ComplexToReal(NaturalLog(complex_t(x, 0)));
}
bool IsNaN(complex_t x) {
    return IsNaN(x.real()) || IsNaN(x.imag());
}
bool IsInf(complex_t x) {
    return IsInf(x.real()) || IsInf(x.imag());
}
num_t Exp(num_t x) {
    ComplexToReal(Exp(complex_t(x, 0)));
}
#ifdef Freestanding
complex_t Exp(complex_t x) {
    return Pow(e, x);
}
#endif
num_t Ceil(num_t x) {
    return Floor(x) + 1;
}
num_t Sin(num_t x) {
    ComplexToReal(Sin(complex_t(x, 0)));
}
num_t InversedSin(num_t x) {
    ComplexToReal(InversedSin(complex_t(x, 0)));
}