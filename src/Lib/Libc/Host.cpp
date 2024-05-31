#ifndef Freestanding
#include "../Host.hpp"

std::ostream& operator<<(std::ostream& stream, const String& string) {
    return stream << string.ToString();
}
std::ostream& operator<<(std::ostream& stream, const Printable& printable) {
    return stream << printable.ToString();
}
void Panic(String str) {
    throw std::runtime_error(str.ToString());
}
num_t StringToNumber(String str) {
    return std::stold(str.ToString());
}
String ToString(num_t x) {
    const ssize_t ix = (ssize_t)x;
    if ((num_t)ix == x) return std::to_string(ix);
    else return std::to_string(x);
}
num_t RandomFloat(void) {
    return (num_t)rand() / RAND_MAX;
}
num_t Abs(complex_t x) {
    return std::abs(x);
}
complex_t Pow(complex_t x, complex_t y) {
    return std::pow(x, y);
}
num_t Cbrt(num_t x) {
    return std::cbrt(x);
}
complex_t NaturalLog(complex_t x) {
    return std::log(x);
}
bool IsNaN(num_t x) {
    return std::isnan(x);
}
bool IsInf(num_t x) {
    return std::isinf(x);
}
complex_t Exp(complex_t x) {
    return std::exp(x);
}
num_t Round(num_t x) {
    return std::round(x);
}
num_t Gamma(num_t x) {
    return std::tgamma(x);
}
num_t Floor(num_t x) {
    return std::floor(x);
}
complex_t Sin(complex_t x) {
    return std::sin(x);
}
complex_t InversedSin(complex_t x) {
    return std::asin(x);
}
num_t InversedTan2(num_t y, num_t x) {
    return std::atan2(y, x);
}

#endif