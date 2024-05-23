#include "Host.hpp"

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
#ifndef Freestanding
num_t Abs(complex_t x) {
    return std::abs(x);
}
#endif
num_t Max(num_t x, num_t y) {
    return (x <= y) ? y : x;
}
#ifndef Freestanding
complex_t Pow(complex_t x, complex_t y) {
    return std::pow(x, y);
}
#endif
num_t Pow(num_t x, num_t y) {
    return Pow(complex_t(x, 0), complex_t(y, 0)).real();
}
complex_t Sqrt(complex_t x) {
    return Pow(x, 0.5);
}
num_t Sqrt(num_t x) {
    return Pow(x, 0.5);
}
#ifndef Freestanding
num_t NaturalLog(num_t x) {
    return std::log(x);
}
complex_t NaturalLog(complex_t x) {
    return std::log(x);
}
bool IsNaN(num_t x) {
    return std::isnan(x);
}
#endif
bool IsNaN(complex_t x) {
    return IsNaN(x.real()) || IsNaN(x.imag());
}
#ifndef Freestanding
bool IsInf(num_t x) {
    return std::isinf(x);
}
#endif
bool IsInf(complex_t x) {
    return IsInf(x.real()) || IsInf(x.imag());
}