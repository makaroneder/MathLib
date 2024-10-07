#include "Host.hpp"
#include "Math/Trigonometry.hpp"

#define ComplexToReal(x)                                                \
    const complex_t complex = x;                                        \
    return FloatsEqual<num_t>(complex.GetImaginary(), 0) ? complex.GetReal() : MakeNaN()

[[noreturn]] void Panic(String str) {
    Panic(str.GetValue());
}
char ToUpper(char chr) {
    if (IsLower(chr)) return chr + 'A' - 'a';
    else return chr;
}
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
bool IsWhiteSpace(char chr) {
    return chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r';
}
bool IsHexDigit(char chr) {
    return IsDigit(chr) || (chr >= 'a' && chr <= 'f') || (chr >= 'A' && chr <= 'F');
}
bool IsAlphaDigit(char chr) {
    return IsAlpha(chr) || IsDigit(chr);
}
size_t BitwiseAnd(size_t x, size_t y) {
    return x & y;
}
size_t BitwiseOr(size_t x, size_t y) {
    return x | y;
}
size_t BitwiseXor(size_t x, size_t y) {
    return x ^ y;
}
size_t BitwiseNot(size_t x) {
    return ~x;
}
size_t BitwiseNand(size_t x, size_t y) {
    return ~(x & y);
}
size_t BitwiseNor(size_t x, size_t y) {
    return ~(x | y);
}
size_t BitwiseXnor(size_t x, size_t y) {
    return ~(x ^ y);
}
num_t Abs(num_t x) {
    return x < 0 ? -x : x;
}
num_t Max(num_t x, num_t y) {
    return (x < y) ? y : x;
}
num_t Min(num_t x, num_t y) {
    return (x < y) ? x : y;
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
    return IsNaN(x.GetReal()) || IsNaN(x.GetImaginary());
}
bool IsInf(complex_t x) {
    return IsInf(x.GetReal()) || IsInf(x.GetImaginary());
}
complex_t Exp(complex_t x) {
    return (complex_t(0, Sin(x.GetImaginary())) + Cos<num_t>(x.GetImaginary())) * Exp(x.GetReal());
}
num_t Ceil(num_t x) {
    return Floor(x) + 1;
}
num_t Sin(num_t x) {
    ComplexToReal(Sin(complex_t(x, 0)));
}
num_t InversedSin(num_t x) {
    ComplexToReal(InversedSin(complex_t(x, 0)));
}