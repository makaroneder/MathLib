#ifndef Trigonometry_H
#define Trigonometry_H
#include "Constants.hpp"

/// @brief cot(x) = cos(x) / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Cotangent of x
template <typename T>
constexpr T Cot(T x) {
    return std::cos(x) / std::sin(x);
}
/// @brief sec(x) = 1 / cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Secant of x
template <typename T>
constexpr T Sec(T x) {
    const T one = 1;
    return one / std::cos(x);
}
/// @brief csc(x) = 1 / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Cosecant of x
template <typename T>
constexpr T Csc(T x) {
    const T one = 1;
    return one / std::sin(x);
}
/// @brief crd(x) = 2sin(x / 2)
/// @tparam T Type of number
/// @param x Radians
/// @return Chord of x
template <typename T>
constexpr T Crd(T x) {
    const T two = 2;
    return two * std::sin(x / two);
}
/// @brief versin(x) = 1 - cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Versed sine of x
template <typename T>
constexpr T Versin(T x) {
    const T one = 1;
    return one - std::cos(x);
}
/// @brief coversin(x) = 1 - sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Coversed sine of x
template <typename T>
constexpr T Coversin(T x) {
    const T one = 1;
    return one - std::sin(x);
}
/// @brief vercos(x) = 1 + cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Versed cosine of x
template <typename T>
constexpr T Vercos(T x) {
    const T one = 1;
    return one + std::cos(x);
}
/// @brief covercos(x) = 1 + sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Coversed cosine of x
template <typename T>
constexpr T Covercos(T x) {
    const T one = 1;
    return one + std::sin(x);
}
/// @brief haversin(x) = (1 - cos(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Haversed sine of x
template <typename T>
constexpr T Haversin(T x) {
    const T one = 1;
    const T two = 2;
    return (one - std::cos(x)) / two;
}
/// @brief hacoversin(x) = (1 - sin(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hacoversed sine of x
template <typename T>
constexpr T Hacoversin(T x) {
    const T one = 1;
    const T two = 2;
    return (one - std::sin(x)) / two;
}
/// @brief havercos(x) = (1 + cos(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Haversed cosine of x
template <typename T>
constexpr T Havercos(T x) {
    const T one = 1;
    const T two = 2;
    return (one + std::cos(x)) / two;
}
/// @brief hacovercos(x) = (1 + sin(x)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Hacoversed cosine of x
template <typename T>
constexpr T Hacovercos(T x) {
    const T one = 1;
    const T two = 2;
    return (one + std::sin(x)) / two;
}
/// @brief exsec(x) = versin(x) / cos(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Exsecant of x
template <typename T>
constexpr T Exsec(T x) {
    return Versin<T>(x) / std::cos(x);
}
/// @brief excsc(x) = coversin(x) / sin(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Excosecant of x
template <typename T>
constexpr T Excsc(T x) {
    return Coversin<T>(x) / std::sin(x);
}
/// @brief coth(x) = (e^2x + 1) / (e^2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic cotangent of x
template <typename T>
constexpr T HyperbolicCot(T x) {
    const T one = 1;
    const T two = 2;
    return (exp(two * x) + one) / (exp(two * x) - one);
}
/// @brief sech(x) = 2e^x / (e^2x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic secant of x
template <typename T>
constexpr T HyperbolicSec(T x) {
    const T one = 1;
    const T two = 2;
    return (exp(x) * two) / (exp(two * x) + one);
}
/// @brief csch(x) = 2e^x / (e^2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Hyperbolic cosecant of x
template <typename T>
constexpr T HyperbolicCsc(T x) {
    const T one = 1;
    const T two = 2;
    return (exp(x) * two) / (exp(two * x) - one);
}
/// @brief arccot(x) = pi / 2 - arctan(x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed cotangent of x
template <typename T>
constexpr T InversedCot(T x) {
    const T two = 2;
    return pi / two - std::atan(x);
}
/// @brief arcsec(x) = arccos(1 / x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed secant of x
template <typename T>
constexpr T InversedSec(T x) {
    const T one = 1;
    return std::acos(one / x);
}
/// @brief arccsc(x) = arcsin(1 / x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed cosecant of x
template <typename T>
constexpr T InversedCsc(T x) {
    const T one = 1;
    return std::asin(one / x);
}
/// @brief arccrd(x) = 2arcsin(x / 2)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed chord of x
template <typename T>
constexpr T InversedCrd(T x) {
    const T two = 2;
    return two * std::asin(x / two);
}
/// @brief arcexsec(x) = arcsec(x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed exsecant of x
template <typename T>
constexpr T InversedExsec(T x) {
    const T one = 1;
    return InversedSec<T>(x + one);
}
/// @brief arcexcsc(x) = arccsc(x + 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed excosecant of x
template <typename T>
constexpr T InversedExcsc(T x) {
    const T one = 1;
    return InversedCsc<T>(x + one);
}
/// @brief arcversin(x) = arccos(1 - x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed versed sine of x
template <typename T>
constexpr T InversedVersin(T x) {
    const T one = 1;
    return std::acos(one - x);
}
/// @brief arccoversin(x) = arcsin(1 - x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed coversed sine of x
template <typename T>
constexpr T InversedCoversin(T x) {
    const T one = 1;
    return std::asin(one - x);
}
/// @brief arcvercos(x) = arccos(x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed versed cosine of x
template <typename T>
constexpr T InversedVercos(T x) {
    const T one = 1;
    return std::acos(x - one);
}
/// @brief arccovercos(x) = arcsin(x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed coversed cosine of x
template <typename T>
constexpr T InversedCovercos(T x) {
    const T one = 1;
    return std::asin(x - one);
}
/// @brief archaversin(x) = arccos(1 - 2x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed haversed sine of x
template <typename T>
constexpr T InversedHaversin(T x) {
    const T one = 1;
    const T two = 2;
    return std::acos(one - two * x);
}
/// @brief archacoversin(x) = arcsin(1 - 2x)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hacoversed sine of x
template <typename T>
constexpr T InversedHacoversin(T x) {
    const T one = 1;
    const T two = 2;
    return std::asin(one - two * x);
}
/// @brief archavercos(x) = arccos(2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed haversed cosine of x
template <typename T>
constexpr T InversedHavercos(T x) {
    const T one = 1;
    const T two = 2;
    return std::acos(two * x - one);
}
/// @brief archacovercos(x) = arcsin(2x - 1)
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hacoversed cosine of x
template <typename T>
constexpr T InversedHacovercos(T x) {
    const T one = 1;
    const T two = 2;
    return std::asin(two * x - one);
}
/// @brief arccoth(x) = ln((x + 1) / (x - 1)) / 2
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic cotangent of x
template <typename T>
constexpr T InversedHyperbolicCot(T x) {
    const T one = 1;
    const T two = 2;
    return std::log((x + one) / (x - one)) / two;
}
/// @brief arcsech(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 - 1)))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic secant of x
template <typename T>
constexpr T InversedHyperbolicSec(T x) {
    const T one = 1;
    return std::acosh(one / x);
}
/// @brief arccsch(x) = ln((1 / x) + sqrt(2 $ (1 / x^2 + 1)))
/// @tparam T Type of number
/// @param x Radians
/// @return Inversed hyperbolic cosecant of x
template <typename T>
constexpr T InversedHyperbolicCsc(T x) {
    const T one = 1;
    return std::log(one / x + std::sqrt(one / std::pow(x, 2) + one));
}

#endif